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

#include "extract_name_value.h"
#include "handler.h"
void
handler(
    struct evhttp_request *req,
    void *arg
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  char  api[MAX_LEN_API+1];
  memset(api, 0, MAX_LEN_API+1);

  char args[MAX_LEN_ARGS+1]; 
  memset(args, 0, MAX_LEN_ARGS+1);

  char outbuf[MAX_LEN_OUTPUT+1];
  memset(outbuf, '\0', MAX_LEN_OUTPUT+1); // TOOD P4 not needed

  char errbuf[MAX_LEN_ERROR+1];
  memset(errbuf, '\0', MAX_LEN_ERROR+1); // TOOD P4 not needed

  web_response_t web_response; 
  memset(&web_response, 0, sizeof(web_response_t));

  char *decoded_uri = NULL;
  char *body = NULL;  uint32_t n_body = 0;
  struct evbuffer *reply = NULL;

  if ( arg == NULL ) { go_BYE(-1); } 
  web_info_t *web_info = (web_info_t *)arg;
  struct event_base *base = web_info->base;
  if ( base == NULL ) { go_BYE(-1); }
  reply = evbuffer_new();
  if ( reply == NULL) { go_BYE(-1); }
  int uidx = -1; 

  if ( web_info->login_endp == NULL ) { go_BYE(-1); } 
  if ( web_info->login_page == NULL ) { go_BYE(-1); } 
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
      if ( L != NULL ) { lua_close(L); }
      // Internals of sess_state should be freed by user
      memset(&(web_info->sess_state[i]), 0, sizeof(sess_state_t));
    }
  }
  //----------------------------------------------------------
  /* JUST FOR CLASS 
  const char * client = req->remote_host; 
  if ( client != NULL ) { 
    fprintf(stderr, "Client IP address=%s\n", client);
  }
  */
  // Get URI 
  const char *uri = evhttp_request_uri(req);
  decoded_uri = evhttp_decode_uri(uri);
  if ( decoded_uri == NULL ) { go_BYE(-1); }
  //- Get API 
  status = extract_api_args(decoded_uri, api, MAX_LEN_API, args, 
      MAX_LEN_ARGS);
  free_if_non_null(decoded_uri);
  status = get_body(req, &body, &n_body); cBYE(status);
  // printf("api = %s \n", api);
  // printf("args = %s \n", args);
  // printf("body = %s \n", body);
  cBYE(status);
  /* TODO JUst for class 
  if ( req_type  == 0 ) {  // unknown endpoint: redirect to home
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Location", "Static?home.html"); 
    evhttp_send_reply(req, HTTP_MOVETEMP, "ERROR", reply);
    if ( reply != NULL ) { evbuffer_free(reply); reply = NULL; }
    return;
  }
  */
  // Deal with what happens when user comes to login page 
  if ( strcasecmp(api, web_info->login_endp) == 0 ) {
    const char *info = NULL;
    if ( ( body != NULL ) && ( *body != '\0' ) ) { 
      info = body;
    }
    else {
      info = args;
    }
    char uname[MAX_LEN_USER_NAME+1]; 
    memset(uname, 0, MAX_LEN_USER_NAME+1);
    status = extract_name_value(info, "User=", '&', uname, MAX_LEN_USER_NAME); 
    cBYE(status);
    if ( *uname == '\0' ) { go_BYE(-1); } 
    for ( uint32_t i = 0; i < web_info->n_users; i++ ) { 
      if ( strcmp(uname, web_info->users[i]) == 0 ) {
        uidx = (int)i; break;
      }
    }
    if ( uidx < 0 ) { 
      strcpy(errbuf, "{\"Error\" : \"Invalid Credentials\"}");
      go_BYE(-1); 
    }
    // Create a session for this user if one doesn't exist
    if ( web_info->sess_state[uidx].L == NULL ) { 
      lua_State *L = mk_lua_state();
      web_info->sess_state[uidx].L = L;
      if ( web_info->init_lua_state != NULL ) { 
        status = luaL_dostring(L, web_info->init_lua_state);
        if ( status != 0 ) { 
          fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
        }
        cBYE(status); 
      }
      web_info->sess_state[uidx].t_create = 
        web_info->sess_state[uidx].t_touch = get_time_usec();
      web_info->sess_state[uidx].sess_hash = RDTSC(); // TODO P1
    }
    char cookie[MAX_LEN_COOKIE+1];
    sprintf(cookie, "sessionID=%" PRIu64 "; ", 
      web_info->sess_state[uidx].sess_hash);
    evhttp_add_header(evhttp_request_get_output_headers(req),
          "Set-Cookie", cookie);
    sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
    goto BYE;
  }
  // Get Cookie. If none, redirect to login page 
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
    break;
  }
  if ( uidx < 0 ) { 
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Location", web_info->login_page);
    evhttp_send_reply(req, HTTP_MOVETEMP, "ERROR", reply);
    // release any resources allocated 
    if ( reply != NULL ) { evbuffer_free(reply); reply = NULL; }
    return; 
  }

  if ( strcmp(api, "Halt") == 0 ) {
    // Inform all threads that they need to terminate 
    int l_expected = 0;
    int l_desired  = 1;
    bool rslt = __atomic_compare_exchange(&(web_info->halt), 
        &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( !rslt ) { go_BYE(-1); }

    /* TODO: P4 Need to get loopbreak to wait for these 3 statements
    evbuffer_add_printf(reply, "\{ \"Server\" : \"Halting\"}\n"); 
    evhttp_send_reply(req, HTTP_OK, "OK", reply);
    evbuffer_free(reply);
    */
    event_base_loopbreak(base);
  }

  proc_req_fn_t process_req_fn = web_info->proc_req_fn;
  status = process_req_fn(uidx, api, args, body, n_body, web_info,
      outbuf, MAX_LEN_OUTPUT, errbuf, MAX_LEN_ERROR, &web_response);
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
  if ( web_response.is_set ) {
    if ( web_response.file_name == NULL ) { go_BYE(-1); } 
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

    if ( web_response.is_err ) { 
      evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", reply);
    }
    else {
      evhttp_send_reply(req, HTTP_OK, "OK", reply);
    }
    goto BYE; 
  }
BYE:
  if ( status == 0 ) { 
    evbuffer_add_printf(reply, "%s", outbuf); 
    evhttp_send_reply(req, HTTP_OK, "OK", reply);
  }
  else {
    evbuffer_add_printf(reply, "%s", errbuf); 
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", reply);
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
}
