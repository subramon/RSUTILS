#ifndef __CSP_INFO_H
#define __CSP_INFO_H
//START_FOR_CDEF
typedef enum {
  csp_static=1,
  csp_Lua_func=2,
  csp_C_func=3,
} csp_src_t;

typedef struct _csp_static_t {
  char *str;
  uint32_t len;
} csp_static_t;

typedef struct _csp_dynamic_t {
  char *func;
  char *args;
  char *dummy; // sample code 
  char *cache;  // length of cache provided by len below 
  uint32_t len;
} csp_dynamic_t;

typedef struct _csp_frag_t { // frag == fragment
  union { 
    csp_static_t static_str;
    csp_dynamic_t Lua_str;
    csp_dynamic_t C_str;
  } frag;
  csp_src_t csp_src;
} csp_frag_t;
typedef struct _csp_info_t {
  csp_frag_t *csp_frag; // [n]
  char **frag_label; // label of frag, must be unique, can be null
  int n;
  int sz; // sz >= n
} csp_info_t;
//STOP_FOR_CDEF
#endif //  __CSP_INFO_H
