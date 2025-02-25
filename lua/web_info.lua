 return [[ 

typedef struct _web_response_t {
  char *file_name;
  bool delete_file;



  char **header_key;
  char **header_val;
  int num_headers;
  bool is_set;
  bool is_err;
  int response_code;
} web_response_t;



typedef int (*sess_clean_fn_t)(
    void *L
    );
typedef int (*proc_req_fn_t)(
    int user_id,
    const char *const api,
    const char *args,
    const char *body,
    uint32_t n_body,
    void *W,
    char *outbuf,
    size_t sz_outbuf,
    char *errbuf,
    size_t sz_errbuf,
    web_response_t *ptr_web_response
    );


typedef struct _sess_state_t {
  void *L;
  uint64_t sess_hash;
  uint64_t t_create;
  uint64_t t_touch;
} sess_state_t;

typedef struct _web_info_t {
  struct event_base **bases;
  proc_req_fn_t proc_req_fn;
  sess_clean_fn_t sess_clean_fn;

  int port;
  int n_threads;
  char *docroot;
  char *login_page;
  char *home_page;
  char *login_endp;
  char *logout_endp;
  uint32_t max_headers_size;
  uint32_t max_body_size;
  bool is_external;
  bool is_cors;


  char *init_lua_state;
  uint32_t timeout_sec;
  uint32_t n_sessions;


  char **users;
  sess_state_t *sess_state;
  int *in_use;
  uint32_t n_users;
  void *X;
  int halt;
} web_info_t;

 ]] 
