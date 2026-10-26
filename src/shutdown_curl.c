#include <curl/curl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "setup_curl.h"
#include "shutdown_curl.h"

int 
shutdown_curl(
    void *ptr_userdata,
    void **ptr_headers,
    CURL **ptr_ch
    )
{

  if ( ptr_userdata != NULL ) { 
    if ( ptr_userdata != NULL ) { 
      curl_userdata_t *U = (curl_userdata_t *)ptr_userdata;
      free_if_non_null(U->base);
      memset(U, 0, sizeof(curl_userdata_t));
    }
    ptr_userdata = NULL;
  }
  if ( ptr_headers != NULL ) { 
    struct curl_slist *headers = *ptr_headers;
    if ( headers != NULL ) { 
      curl_slist_free_all(headers);
      *ptr_headers = NULL; 
    }
    *ptr_headers = NULL;
  }
  if ( ptr_ch == NULL ) { 
    CURL *ch = *ptr_ch;
    if ( ch != NULL ) { curl_easy_cleanup(ch);  ch = NULL; }
    *ptr_ch = NULL;
  }
  curl_global_cleanup(); 
  return 0;
}
