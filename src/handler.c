#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "web_struct.h"
#include "get_file_size.h"
#include "rs_mmap.h"
#include "extract_api_args.h"
#include "get_body.h"
#include "mk_lua_state.h"
#include "get_time_usec.h"
#include "rdtsc.h"
#include "lookup8.h"
#include "multiple.h"

#include "extract_name_value.h"
#include "extract_json_value.h"
#include "handler.h"
void
handler(
    struct evhttp_request *req,
    void *arg
    )
{
  int status = 0;
  char *outbuf = NULL; 
  char *errbuf = NULL; 
  char *X = NULL; size_t nX = 0;
  char  api[MAX_LEN_API+1];
  memset(api, 0, MAX_LEN_API+1);

  char args[MAX_LEN_ARGS+1]; 
  memset(args, 0, MAX_LEN_ARGS+1);

  web_response_t web_response; 
  memset(&web_response, 0, sizeof(web_response_t));

  char *decoded_uri = NULL;
  char *body = NULL;  uint32_t n_body = 0;
  struct evbuffer *reply = NULL;

  if ( arg == NULL ) { go_BYE(-1); } 
  web_info_t *web_info = (web_info_t *)arg;
  reply = evbuffer_new();
  if ( reply == NULL) { go_BYE(-1); }
  int uidx = -1; 
  //----------------------------------------------
  if ( web_info->outbuf_size > 0 ) { 
    outbuf = malloc(web_info->outbuf_size);
    bzero(outbuf, web_info->outbuf_size);
  }
  else {
    outbuf = malloc(MAX_LEN_OUTPUT+1);
    bzero(outbuf, MAX_LEN_OUTPUT+1);
  }
  //----------------------------------------------
  if ( web_info->errbuf_size > 0 ) { 
    errbuf = malloc(web_info->errbuf_size);
    bzero(errbuf, web_info->errbuf_size);
  }
  else {
    errbuf = malloc(MAX_LEN_ERROR+1);
    bzero(errbuf, MAX_LEN_ERROR+1);
  }
  //----------------------------------------------

  // Delete old sessions
  for ( uint32_t i = 0; i < web_info->n_users; i++ ) {
    uint64_t t_touch = web_info->sess_state[i].t_touch;
    if ( t_touch == 0 ) { continue; }
    uint64_t t_now = get_time_usec();
    if ( t_now < t_touch ) { go_BYE(-1); }
    uint64_t t = (t_now - t_touch)/1000000;
    if ( web_info->timeout_sec == 0 ) { continue; } // no timeout
    if ( t > web_info->timeout_sec ) { 
      lua_State *L = web_info->sess_state[i].L;
      if ( L != NULL ) { 
        sess_clean_fn_t sess_clean_fn = web_info->sess_clean_fn;
        sess_clean_fn(L); 
        lua_close(L); 
      }
      // Internals of sess_state should be freed by user
      memset(&(web_info->sess_state[i]), 0, sizeof(sess_state_t));
    }
  }
  // Get URI 
  const char *uri = evhttp_request_uri(req);
  decoded_uri = evhttp_decode_uri(uri);
  if ( decoded_uri == NULL ) { go_BYE(-1); }
  //- Get API 
  status = extract_api_args(decoded_uri, api, MAX_LEN_API, args, 
      MAX_LEN_ARGS);
  free_if_non_null(decoded_uri);
  status = get_body(req, &body, &n_body); cBYE(status);
  cBYE(status);
  // printf("api = %s \n", api);
  // printf("args = %s \n", args);
  // printf("body = %s \n", body);
  if ( *api == '\0' ) { 
    strcpy(errbuf, " {\"API\" : \"Not found\" } "); go_BYE(-1); 
  }
  // START: Deal with what happens when user comes to login page 
  if ( ( web_info->login_endp != NULL ) && 
       ( strcasecmp(api, web_info->login_endp) == 0 ) ) {
    char uname[MAX_LEN_USER_NAME+1]; 
    memset(uname, 0, MAX_LEN_USER_NAME+1);
    if ( ( body != NULL ) && ( *body != '\0' ) ) { 
      status = extract_json_value(body, "User", uname, MAX_LEN_USER_NAME); 
      cBYE(status);
    }
    else {
      if ( ( args == NULL ) || ( *args == '\0' ) ) { 
        strcpy(errbuf, "{\"Error\":\"Invalid Credentials\"}"); go_BYE(-1); 
      }
      status = extract_name_value(args, "User=", '&', uname, MAX_LEN_USER_NAME); 
      cBYE(status);
    }
    if ( *uname == '\0' ) { 
      strcpy(errbuf, "{\"Error\" : \"Invalid Credentials\"}"); go_BYE(-1); 
    } 
    for ( uint32_t i = 0; i < web_info->n_users; i++ ) { 
      if ( strcmp(uname, web_info->users[i]) == 0 ) {
        uidx = (int)i; break;
      }
    }
    if ( uidx < 0 ) { 
      strcpy(errbuf, "{\"Error\" : \"Invalid Credentials\"}"); go_BYE(-1); 
    }
    // Create a session for this user if one doesn't exist
    uint64_t sess_hash = 0;
    if ( web_info->sess_state[uidx].L == NULL ) {
      size_t sz = sizeof(sess_state_t);
      sess_state_t zero; memset(&zero, 0, sz);
      if ( memcmp(&(web_info->sess_state[uidx]), &zero, sz) != 0 ) { 
        go_BYE(-1); // Bad logic on our side
      }
      lua_State *L = mk_lua_state();
      if ( web_info->init_lua_state != NULL ) { 
        status = luaL_dostring(L, web_info->init_lua_state);
        if ( status != 0 ) { 
          fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
        }
        cBYE(status); 
      }
      web_info->sess_state[uidx].L = L;
      uint64_t t = get_time_usec();
      web_info->sess_state[uidx].t_create =  t;
      web_info->sess_state[uidx].t_touch = t;

      // START: Create a hash for this session
      pthread_t tid = pthread_self();
      struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
      const char *dval = evhttp_find_header(headers, "Date");
      size_t len = 64; 
      if ( dval != NULL ) { len += strlen(dval); }
      const char * client = req->remote_host; 
      if ( client != NULL ) { len += strlen(client); }
      len = multiple_n(len, 8); // needed for hash2()
      char *hbuf = malloc(len); memset(hbuf, 0, len);
      sprintf(hbuf, "%ld_%s_%s_%" PRIu64 "\n", tid, dval, client, RDTSC());
      sess_hash = hash2((ub8 *)hbuf, len/8, 1234566789);
      free_if_non_null(hbuf);
      web_info->sess_state[uidx].sess_hash = sess_hash;
      // STOP: Create a hash for this session
    }
    else {
      sess_hash = web_info->sess_state[uidx].sess_hash;
      web_info->sess_state[uidx].t_touch = get_time_usec();
    }
    char cookie[MAX_LEN_COOKIE+1];
    sprintf(cookie, "sessionID=%" PRIu64 "; ", 
      web_info->sess_state[uidx].sess_hash);
    evhttp_add_header(evhttp_request_get_output_headers(req),
          "Set-Cookie", cookie);
    evbuffer_add_printf(reply, "{ \"Login\" : \"Successful\"}\n"); 
    if ( web_info->home_page != NULL ) { 
      evhttp_add_header(evhttp_request_get_output_headers(req),
          "Location", web_info->home_page);
      evhttp_send_reply(req, HTTP_MOVETEMP, "Login successful", reply);
    }
    else {
      evhttp_send_reply(req, HTTP_OK, "OK", reply);
    }
    evbuffer_free(reply); 
    return;
  }
  // STOP : Deal with what happens when user comes to login page 
  // Get Cookie. If none, redirect to login page 
  uidx = -1; // => we have not recognized user
  for ( ; ; ) {
    struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
    const char *hval = evhttp_find_header(headers, "Cookie");
    if ( hval == NULL ) { break; }
    char buf[MAX_LEN_COOKIE+1]; memset(buf, 0, MAX_LEN_COOKIE+1);
    status = extract_name_value(hval, "sessionID=", ';', 
        buf, MAX_LEN_COOKIE);
    if ( *buf == '\0' ) {  break; }
    char *endptr = NULL; 
    uint64_t sess_hash = strtoull(buf, &endptr, 10);
    if ( *endptr != '\0' ) { break; }
    // Check if valid sess_hash_
    for ( uint32_t i = 0; i < web_info->n_users; i++ ) { 
      if ( sess_hash == web_info->sess_state[i].sess_hash ) { 
        uidx = (int)i; break;
      }
    }
    if ( uidx < 0 ) { break; }
    // Check if this user has an active request 
    int l_expected = 0;
    int l_desired  = 1;
    // printf("Trying to lock user %d \n", uidx);
    bool rslt = __atomic_compare_exchange(&(web_info->in_use[uidx]), 
        &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( !rslt ) { 
      evbuffer_add_printf(reply, "{ \"Server\" : \"Busy\"}\n"); 
      evhttp_send_reply(req, HTTP_SERVUNAVAIL, "SERVER_BUSY", reply);
      evbuffer_free(reply);
      return;
    }
    break;
  }
  /*
  // Do not allow access to any APIs if user not validated 
  This code has been commented to allows access to some resources
  if ( uidx < 0 ) {
    if ( web_info->login_page == NULL ) { go_BYE(-1); } 
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Location", web_info->login_page);
    evbuffer_add_printf(reply, "{ \"Login\" : \"Invalid\"}\n"); 
    evhttp_send_reply(req, HTTP_MOVETEMP, "ERROR", reply);
    evbuffer_free(reply);
    return; 
  }
  */

  // START: Deal with logout 
  if ( ( web_info->logout_endp != NULL ) &&
       ( strcasecmp(api, web_info->logout_endp) == 0 ) ) {
    if ( uidx >= 0 ) { 
      lua_State *L = web_info->sess_state[uidx].L;
      if ( L != NULL ) { 
        sess_clean_fn_t sess_clean_fn = web_info->sess_clean_fn;
        sess_clean_fn(L); 
        lua_close(L); 
      }
      int l_expected = 1;
      int l_desired  = 0;
      // printf("Freeing user %d \n", uidx);
      bool rslt = __atomic_compare_exchange(&(web_info->in_use[uidx]), 
          &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
      if ( !rslt ) { go_BYE(-1); }
      memset(&(web_info->sess_state[uidx]), 0, sizeof(sess_state_t));
      evbuffer_add_printf(reply, "{ \"Logout\" : \"%d\"}\n", uidx); 
      evhttp_send_reply(req, HTTP_OK, "OK", reply);
      evbuffer_free(reply);
      return;
    }
  }
  // STOP : Deal with logout 
  proc_req_fn_t process_req_fn = web_info->proc_req_fn;
  status = process_req_fn(uidx, api, args, body, n_body, web_info,
      outbuf, MAX_LEN_OUTPUT, errbuf, MAX_LEN_ERROR, &web_response);
  // We can accept a new connection for this user now 
  // So, free the session state for this user 
  if ( uidx >= 0 ) { 
    int l_expected = 1;
    int l_desired  = 0;
    // printf("Freeing user %d \n", uidx);
    bool rslt = __atomic_compare_exchange(&(web_info->in_use[uidx]), 
        &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( !rslt ) { go_BYE(-1); }
  }
  // IMPORTANT: We deal with Halt in 2 places. 
  // (1) process_req_fn()
  // (2) right here. 
  // The first is used to inform other threads that they should stop
  // The second is used to terminate the webserver thread 
  if ( strcmp(api, "Halt") == 0 ) {
    // Inform all threads that they need to terminate 
    int l_expected = 0;
    int l_desired  = 1;
    bool rslt = __atomic_compare_exchange(&(web_info->halt), 
        &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( !rslt ) { go_BYE(-1); }
    // Save states for each user if one exists
    evbuffer_add_printf(reply, "{ \"Server\" : \"Halting\"}\n"); 
    evhttp_send_reply(req, HTTP_OK, "OK", reply);
    evbuffer_free(reply);
    printf("HALTING!!!!\n");
    for ( int i = 0; i < web_info->n_threads; i++ ) { 
      event_base_loopbreak(web_info->bases[i]);
    }
    return;
  }
  // send the headers if any
  for ( int i = 0; i < web_response.num_headers; i++ ) { 
    evhttp_add_header(evhttp_request_get_output_headers(req),
       web_response.header_key[i], web_response.header_val[i]);
  }
  if ( web_response.num_headers == 0 ) { 
    // this is default header
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Content-Type", "application/json; charset=UTF-8");
  }
  if ( web_info->is_cors ) { 
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Access-Control-Allow-Origin", "*"); 
  }
  // Handle case when something other than default is to be returned
  // Can send back contents of a file or outbuf but not both 
  if ( web_response.is_set ) {
    if ( web_response.file_name != NULL ) {
      if ( web_response.sz_outbuf != 0 ) { go_BYE(-1); } 
      // Running into trouble with add_file 
      // open file for reading 
      // int wfd = open(web_response.file_name, O_RDONLY); 
      // if ( wfd < 0 ) { go_BYE(-1); } 
      // send data in file 
      // status = evbuffer_add_file(reply, wfd, 0, -1); cBYE(status); 
      // close(wfd); 
      status = rs_mmap(web_response.file_name, &X, &nX, 0); cBYE(status);
      status = evbuffer_add(reply, X, nX); cBYE(status);
      mcr_rs_munmap(X, nX);
    }
    else if ( web_response.sz_outbuf > 0 ) { 
      if ( web_response.file_name != NULL ) { go_BYE(-1); } 
      if ( web_response.outbuf == NULL ) { go_BYE(-1); }
      status = evbuffer_add(reply, web_response.outbuf, 
          web_response.sz_outbuf);
      free_if_non_null(web_response.outbuf); 
      cBYE(status);
    }
    else {
      go_BYE(-1); 
    }
    goto BYE; 
  }
BYE:
  if ( ( status != 0 ) || 
      ( ( web_response.is_set ) && ( web_response.is_err) ) ) { 
    if ( *errbuf == '\0' ) { //  generic error message
      sprintf(errbuf, " { \"%s\" : \"Error\"  } ", api);
    }
    evbuffer_add_printf(reply, "%s", errbuf); 
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", reply);
  }
  else {
    evbuffer_add_printf(reply, "%s", outbuf); 
    evhttp_send_reply(req, HTTP_OK, "OK", reply);
  }
  // Release resources 
  if ( reply != NULL ) { evbuffer_free(reply); reply = NULL; }
  free_if_non_null(decoded_uri);
  mcr_rs_munmap(X, nX);
  // free resources in web response
  if ( web_response.file_name != NULL ) { 
    if ( web_response.delete_file ) { 
      remove(web_response.file_name);
    }
    free_if_non_null(web_response.file_name);
  }
  for ( int i = 0; i < web_response.num_headers; i++ ) { 
    free_if_non_null(web_response.header_key[i]);
    free_if_non_null(web_response.header_val[i]);
  }
  free_if_non_null(body); 
  free_if_non_null(web_response.header_key); 
  free_if_non_null(web_response.header_val); 
  /* EXPERIMENTAL 
  if ( evhttp_request_is_owned(req) ) { 
    fprintf(stderr, "Deleting req\n");
    evhttp_request_free(req);
  }
  */
  free_if_non_null(outbuf);
  free_if_non_null(errbuf);
}
