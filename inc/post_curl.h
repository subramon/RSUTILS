#include <curl/curl.h>
extern int
execute_curl(
    CURL *ch,
    const char *url,
    const char *const post_data,
    long *ptr_http_code
    );
