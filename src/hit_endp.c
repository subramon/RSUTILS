#include <curl/curl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "isfile.h"
#include "setup_curl.h"
#include "execute_curl.h"
#include "hit_endp.h"

int
hit_endp(
    const char * const protocol,
    const char * const server,
    uint16_t port,
    const char * const api,
    char **ptr_retval
    )
{
  int status = 0;
  CURL *ch = NULL; 
  long http_code = 0;
  curl_userdata_t curl_userdata;
  memset(&curl_userdata, 0, sizeof(curl_userdata_t));
  *ptr_retval = NULL;
  char *retval = NULL;
  char *url = NULL; size_t len = 0;

  status = setup_curl(&curl_userdata, NULL, &ch); cBYE(status);
  len = strlen(protocol) + strlen(server) + strlen(api) + 64;
  url = malloc(len); memset(url, 0, len);
  sprintf(url, "%s://%s:%u/%s\n", protocol, server, port, api);
  status = execute_curl(ch, url, &http_code); cBYE(status);
  if ( http_code != 200 ) { go_BYE(-1); }
  if ( curl_userdata.base != NULL ) { 
    retval = malloc(curl_userdata.size+1);
    memset(retval, 0,  curl_userdata.size+1);
    memcpy(retval, curl_userdata.base, curl_userdata.size);
  }
  *ptr_retval = retval;


BYE:
  free_if_non_null(url);
  free_if_non_null(curl_userdata.base);
  if ( ch != NULL ) { curl_easy_cleanup(ch);  ch = NULL; }
  curl_global_cleanup();
  return status;
}
