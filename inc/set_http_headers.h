#include "web_struct.h"
extern int
set_http_headers(
    const char * const extension,
    int *ptr_nH,
    char ***ptr_hdr_key,
    char ***ptr_hdr_val
    );
extern int
prep_for_file_return(
    char * file_name, 
    web_response_t *ptr_web_response
    );
