#include <curl/curl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "isfile.h"
#include "setup_curl.h"

size_t
WriteMemoryCallback(
    void *contents,
    size_t size,
    size_t nmemb,
    void *userp
    )
{
  size_t bytes_to_write = size * nmemb;
  curl_userdata_t *U = (curl_userdata_t *)userp;

  if ( U->size == 0 ) { 
    if ( U->offset != 0 ) { WHEREAMI; return 0; }
    U->size = 1024; // some initial starting point 
    U->base = malloc(U->size);
    memset(U->base, 0, U->size);
  }

  size_t newsize = U->size; 
  size_t space_left = newsize - U->offset;
  while ( space_left < bytes_to_write ) { 
    newsize*= 2; 
    space_left = newsize - U->offset;
  }
  if ( newsize != U->size ) { 
    U->base = realloc(U->base, newsize);
    U->size = newsize;
  }
  memcpy(U->base+U->offset, contents, bytes_to_write);
  U->offset += bytes_to_write; 
  return bytes_to_write;
}

int
setup_curl(
    void *userdata,
    const char * const cookie_file,
    CURL **ptr_ch
    )
{
  int status = 0;
  CURL *ch = NULL;
  CURLcode res;

  curl_userdata_t *U = (curl_userdata_t *)userdata;
  memset(U, 0, sizeof(curl_userdata_t));
  U->size = 2048;
  U->base = malloc(U->size); 
  memset(U->base, 0, U->size);

  ch = curl_easy_init();
  if ( ch == NULL ) { go_BYE(-1); }
   // follow redirects
  res = curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
  if ( res != CURLE_OK ) { go_BYE(-1); }
  // insecure is okay
  res = curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0);
  if ( res != CURLE_OK ) { go_BYE(-1); }
  res = curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0);
  if ( res != CURLE_OK ) { go_BYE(-1); }

  /* send all data to this function  */
  res = curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  if ( res != CURLE_OK ) { go_BYE(-1); }
  res = curl_easy_setopt(ch, CURLOPT_WRITEDATA, userdata);
  if ( res != CURLE_OK ) { go_BYE(-1); }

  if ( ( cookie_file != NULL ) && ( *cookie_file != '\0' ) ) {
    // make sure cookie_file is valid 
    if ( isfile(cookie_file) ) { 
      FILE *fp = fopen(cookie_file, "w");
      return_if_fopen_failed(fp, cookie_file, "w");
      fclose_if_non_null(fp); 
    }
    res = curl_easy_setopt(ch, CURLOPT_COOKIEFILE, cookie_file);
  }
  *ptr_ch = ch;
BYE:
  return status;
}
