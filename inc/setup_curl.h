// for storing response from curl 
typedef struct _curl_userdata_t { 
  char *base;
  size_t size;
  size_t offset;
} curl_userdata_t;

extern size_t
WriteMemoryCallback(
    void *contents,
    size_t size,
    size_t nmemb,
    void *userp
    );
extern int
setup_curl(
    void *userdata,
    const char * const cookie_file,
    CURL **ptr_ch
    );
