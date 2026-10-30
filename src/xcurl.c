// This is a combination of setup_curl, post_curl and shutdown_curl
// that can be invoked from Lua without any knowledge of curl handles
// More inefficient since we create and close handle each time 
#include <curl/curl.h>
#include <lua.h>
#include "q_macros.h"
#include "txt_to_I8.h"
#include "setup_curl.h"
#include "post_curl.h"
#include "shutdown_curl.h"
#include "execute_curl.h"
#include "xcurl.h"

int
xcurl(
    const char * const url,
    const char * const method,
    const char * const post_data,
    const char ** curl_options, // [n_curl_opts]
    const char ** curl_params, // [n_curl_opts]
    int n_curl_opts, 
    long *ptr_http_code,
    char **ptr_str_response
    )
{
  int status = 0; 
  CURLcode curl_res;
  CURL *ch = NULL; 
  curl_userdata_t userdata; bzero(&userdata, sizeof(userdata));

  status = setup_curl(&userdata, NULL, &ch);  cBYE(status);
  if ( n_curl_opts > 0 ) { 
    if ( curl_options == NULL ) { go_BYE(-1); } 
    if ( curl_params == NULL ) { go_BYE(-1); } 
    // TODO Set options
    for ( int i = 0; i < n_curl_opts; i++ ) { 
      return_if_null_str(curl_options[i]);
      return_if_null_str(curl_params[i]);
      if ( strcmp(curl_options[i], "CURLOPT_FOLLOWLOCATION") == 0 ) { 
        int64_t i8; 
        status = txt_to_I8(curl_params[i], &i8); cBYE(status);
        curl_res = curl_easy_setopt(ch, CURLOPT_TIMEOUT, (long)i8); 
        if ( curl_res != CURLE_OK ) { go_BYE(-1); }
      }
      // TODO P1 Finish other options as well 
      else {
        fprintf(stderr, "Unknown option/param = %s/%s \n", 
            curl_options[i], curl_params[i]);
      }
    }
  }
  if ( strcmp(method, "POST") == 0 ) { 
  status = post_curl(ch, url, post_data, ptr_http_code); cBYE(status); 
  }
  else if ( strcmp(method, "GET") == 0 ) { 
    status = execute_curl(ch, url, ptr_http_code); cBYE(status);
  }
  else {
    fprintf(stderr, "Unknown method %s \n", method);
  }
  cBYE(status); 
  if ( userdata.base != NULL ) { 
    *ptr_str_response = malloc(userdata.size+1);
    memset(*ptr_str_response, 0, userdata.size+1);
    strncpy(*ptr_str_response, userdata.base,userdata.size);
  }

BYE:
  shutdown_curl(&userdata, NULL, &ch);
  return status;
}
