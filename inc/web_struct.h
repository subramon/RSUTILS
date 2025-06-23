#include <lua.h>
#include <evhttp.h>
#include <event2/http.h>
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
  char *outbuf;  // allocated inside, freed by handler 
  uint32_t sz_outbuf;
} web_response_t;

// TODO Why doesnt this work? struct web_info_t; // forward declaration

typedef int (*sess_clean_fn_t)( 
    lua_State *L
    );
typedef int (*proc_req_fn_t)( 
    int user_id,
    const char *const api,
    const char *args,
    const char *body,
    uint32_t n_body,
    void *W, // information for webserver
    char *outbuf, // [sz_outbuf]
    size_t sz_outbuf,
    char *errbuf, // [sz_outbuf]
    size_t sz_errbuf,
    web_response_t *ptr_web_response
    );


typedef struct _sess_state_t {
  lua_State *L;  // all session related info needs to be stored in Lua
  uint64_t sess_hash; // sessionID
  uint64_t t_create; // when created 
  uint64_t t_touch;  // when last accessed
} sess_state_t;

typedef struct _web_info_t {
  struct event_base **bases; // [n_threads]
  proc_req_fn_t proc_req_fn;
  sess_clean_fn_t sess_clean_fn;

  int port;
  uint32_t outbuf_size; // allocated by handler outside 
  uint32_t errbuf_size; // allocated by handler outside
  int n_threads;
  char *docroot; 
  char *login_page;  // e.g., login.html
  char *home_page;  // e.g., home.html
  char *login_endp;  // e.g., Login
  char *logout_endp;  // e.g., Login
  uint32_t max_headers_size; 
  uint32_t max_body_size; 
  bool is_external; // false => accessible only from localhost
  bool is_cors;  // explained below 
  /* See https://portswigger.net/web-security/cors/access-control-allow-origin#:~:text=The%20Access%2DControl%2DAllow%2DOrigin%20header%20is%20included%20in,permitted%20origin%20of%20the%20request.  */

  char *init_lua_state;
  uint32_t timeout_sec;
  uint32_t n_sessions;
  // Currently, there is a 1 to 1 map from user to session
  // In other words, a user cannot have 2 sessions
  char **users; // [n_users];
  sess_state_t *sess_state; // [n_users];
  int *in_use; // [n_users];
  uint32_t n_users;
  void *X; // anything else we want webserver to have 
  int halt;  // set to 1 => all threads must halt 
} web_info_t;
// STOP_FOR_CDEF

#endif //  __WEB_STRUCT_H 
