#include "evhttp.h"
#include "event2/http.h"
#ifndef __WEB_STRUCT_H 
#define __WEB_STRUCT_H 

#define MAX_LEN_FILE_NAME 63
// This is used when we want to return something more complex than
// just text (which is returned in opbuf and/or errbuf)

//START_FOR_CDEF
typedef struct _web_response_t {
  char *file_name;
  bool delete_file;
  // example of a header is Content-Type: image/foo.png
  // example of a header is Content-Type: application/json; charset=UTF-8
  // above is default header 
  char **header_key; // [num_headers][...]
  char **header_val; // [num_headers][...]
  int num_headers; 
  bool is_set; // default false
  bool is_err; // default false
  int response_code;  // see http.h
} web_response_t;

typedef int (*proc_req_fn_t)( 
    const char *const api,
    const char *args,
    const char *body,
    uint32_t n_body,
    void *W,
    char *outbuf, // [sz_outbuf]
    size_t sz_outbuf,
    char *errbuf, // [sz_outbuf]
    size_t sz_errbuf,
    web_response_t *ptr_web_response
    );


typedef struct _web_info_t { 
  struct event_base *base;
  proc_req_fn_t proc_req_fn;

  int port;
  uint32_t max_headers_size; 
  uint32_t max_body_size; 
  bool is_external; // false => accessible only from localhost
  bool is_cors;  // explained below 
  /* See https://portswigger.net/web-security/cors/access-control-allow-origin#:~:text=The%20Access%2DControl%2DAllow%2DOrigin%20header%20is%20included%20in,permitted%20origin%20of%20the%20request.  */

  uint32_t n_sessions;
  char **users; // [n_users];
  uint32_t n_users;
  void *C; // anything else we want webserver to have 
  int halt;  // set to 1 => all threads must halt 
} web_info_t;
// STOP_FOR_CDEF

#endif //  __WEB_STRUCT_H 
