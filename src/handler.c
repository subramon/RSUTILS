#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "web_struct.h"
#include "get_file_size.h"
#include "rs_mmap.h"
#include "extract_api_args.h"
#include "get_body.h"

#include "handler.h"
void
handler(
    struct evhttp_request *req,
    void *arg
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  char *decoded_uri = NULL;
  char  api[MAX_LEN_API+1];
  char args[MAX_LEN_ARGS+1]; memset(args, 0, MAX_LEN_ARGS+1);
  char outbuf[MAX_LEN_OUTPUT+1];
  char errbuf[MAX_LEN_ERROR+1];
  char *body = NULL;  uint32_t n_body = 0;
  memset(outbuf, '\0', MAX_LEN_OUTPUT+1); // TOOD P4 not needed
  memset(errbuf, '\0', MAX_LEN_ERROR+1); // TOOD P4 not needed
  struct evbuffer *opbuf = NULL;
  if ( arg == NULL ) { go_BYE(-1); } 
  web_info_t *web_info = (web_info_t *)arg;
  struct event_base *base = web_info->base;
  if ( base == NULL ) { go_BYE(-1); }
  opbuf = evbuffer_new();
  if ( opbuf == NULL) { go_BYE(-1); }
  const char *uri = evhttp_request_uri(req);
  decoded_uri = evhttp_decode_uri(uri);
  if ( decoded_uri == NULL ) { go_BYE(-1); }
  web_response_t web_response; 
  memset(&web_response, 0, sizeof(web_response_t));
  //--------------------------------------
#define VERBOSE
#ifdef VERBOSE
  const char* hkeys[] = { 
    "Accept",
    "Authorization",
    "Cookie",
    "Content-Type",
    "Host",
    "Referer",
    "User-Agent",
    "X-Forwarded-For", };

  // Parse the query for later lookups
  struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
  for ( int i = 0; i < 8; i++ ) { 
    const char *hval = evhttp_find_header(headers, hkeys[i]);

    if ( hval != NULL ) { 
      fprintf(stderr, "%s:%s \n", hkeys[i], hval);
    }
  }
  const char * client = req->remote_host; 
  if ( client != NULL ) { 
    fprintf(stderr, "Client IP address=%s\n", client);
  }
  /*
  if ( strcmp(host, "127.0.0.1") == 0 ) { 
    evbuffer_add_printf(opbuf, "BUZZ OFF!!!\n");
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", opbuf);
  }
   goto BYE;
  */

#endif
  //--------------------------------------
  status = extract_api_args(decoded_uri, api, MAX_LEN_API, args, 
      MAX_LEN_ARGS);
  free_if_non_null(decoded_uri);
  // printf("api = %s \n", api);
  // printf("args = %s \n", args);
  cBYE(status);
  get_req_fn_t get_req_fn = web_info->get_req_fn;
  int req_type = get_req_fn(api);
  if ( req_type  == 0 ) { go_BYE(-1); }
  if ( strcmp(api, "Halt") == 0 ) {
    // TODO: P4 Need to get loopbreak to wait for these 3 statements
    // evbuffer_add_printf(opbuf, "%s\n", g_rslt);
    // evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
    // evbuffer_free(opbuf);
    event_base_loopbreak(base);
  }
  status = get_body(req, &body, &n_body); cBYE(status);
  if ( n_body > 0 ) { printf("Size of body = %u \n", n_body); }

  void *W = web_info->W; // contains stuff needed by process_req()
  proc_req_fn_t process_req_fn = web_info->proc_req_fn;
  status = process_req_fn(req_type, api, args, body, n_body, W,
      outbuf, MAX_LEN_OUTPUT, errbuf, MAX_LEN_ERROR, &web_response);
  // send the headers if any
  for ( int i = 0; i < web_response.num_headers; i++ ) { 
    evhttp_add_header(evhttp_request_get_output_headers(req),
       web_response.header_key[i], web_response.header_val[i]);
  }
  // TODO P0 START HACK 
  if ( strcmp(api, "Pass") == 0 ) { 
    evhttp_send_reply(req, HTTP_MOVETEMP, "OK", opbuf);
    goto BYE;
  }
  // TODO P0 STOP  HACK 
  // Handle case when something other than default is to be returned
  if ( web_response.is_set ) {
    if ( web_response.file_name == NULL ) { go_BYE(-1); } 
    // Running into trouble with add_file 
    // open file for reading 
    // int wfd = open(web_response.file_name, O_RDONLY); 
    // if ( wfd < 0 ) { go_BYE(-1); } 
    // send data in file 
    // status = evbuffer_add_file(opbuf, wfd, 0, -1); cBYE(status); 
    // close(wfd); 
    status = rs_mmap(web_response.file_name, &X, &nX, 0); cBYE(status);
    status = evbuffer_add(opbuf, X, nX); cBYE(status);
    mcr_rs_munmap(X, nX);

    if ( web_response.is_err ) { 
      evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", opbuf);
    }
    else {
      evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
    }
    goto BYE; 
  }
  else { 
    // this is default header
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Content-Type", "application/json; charset=UTF-8");
  }
  if ( web_info->is_cors ) { 
    evhttp_add_header(evhttp_request_get_output_headers(req),
        "Access-Control-Allow-Origin", "*"); 
  }
  if ( status == 0 ) { 
    evbuffer_add_printf(opbuf, "%s", outbuf); 
    evhttp_send_reply(req, HTTP_OK, "OK", opbuf);
  }
  else {
    evbuffer_add_printf(opbuf, "%s", errbuf); 
    evhttp_send_reply(req, HTTP_BADREQUEST, "ERROR", opbuf);
  }
BYE:
  mcr_rs_munmap(X, nX);
  if ( opbuf != NULL ) { evbuffer_free(opbuf); opbuf = NULL; }
  free_if_non_null(decoded_uri);
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
