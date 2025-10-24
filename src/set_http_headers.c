#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "q_macros.h"
#include "set_http_headers.h"
int
set_http_headers(
    const char * const extension,
    bool is_cache,
    int *ptr_nH,
    char ***ptr_hdr_key,
    char ***ptr_hdr_val
    )
{
  int status = 0;
  int nH = 0;
  char **hdr_key = NULL;
  char **hdr_val = NULL;
  if ( extension == NULL ) { go_BYE(-1); }
  if ( strcmp(extension, "html") == 0 )  {
    nH = 2;
  }
  else if ( strcmp(extension, "css") == 0 )  {
    nH = 1;
  }
  else if ( strcmp(extension, "csv") == 0 )  {
    nH = 1;
  }
  else if ( strcmp(extension, "ico") == 0 )  {
    nH = 1;
  }
  else if ( strcmp(extension, "js") == 0 )  {
    nH = 1; 
  }
  else if ( strcmp(extension, "json") == 0 )  {
    nH = 2; 
  }
  else if ( strcmp(extension, "qdf") == 0 )  {
    nH = 1; 
  }
  if ( nH == 0 ) { return status; }
  if ( is_cache ) { nH++; } 
  //=============================================
  hdr_key = malloc(nH * sizeof(char *));
  memset(hdr_key, 0,  nH * sizeof(char *));
  hdr_val = malloc(nH * sizeof(char *));
  memset(hdr_val, 0,  nH * sizeof(char *));
  if ( strstr(extension, "html") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("text/html");

    hdr_key[1] =  strdup("charset");
    hdr_val[1] = strdup("utf8");
  }
  else if ( strstr(extension, "csv") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("text/csv");
  }
  else if ( strstr(extension, "css") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("text/css");
  }
  else if ( strstr(extension, "ico") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("image/png");
  }
  else if ( strstr(extension, "js") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("text/javascript");
  }
  else if ( strstr(extension, "qdf") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("application/octet-stream");
  }
  if ( strstr(extension, "json") != NULL )  {
    hdr_key[0] =  strdup("Content-Type");
    hdr_val[0] = strdup("text/json");

    hdr_key[1] =  strdup("charset");
    hdr_val[1] = strdup("utf8");
  }
  if ( is_cache ) { 
    hdr_key[nH-1] = strdup("Cache-Control");
    hdr_val[nH-1] = strdup("public, max-age=31536000, immutable");
  }
  *ptr_nH = nH;
  *ptr_hdr_key = hdr_key;
  *ptr_hdr_val = hdr_val;
BYE:
  return status;
}
#include "web_struct.h"
#include "isfile.h"
#include "extract_extension.h"
int
prep_for_file_return(
    char * file_name, 
    bool is_cache, 
    web_response_t *ptr_web_response
    )
{
  int status = 0;
  char *extension = NULL;

  if ( !isfile(file_name) ) { 
    fprintf(stderr, "File [%s] not found \n", file_name); go_BYE(-1); 
  }
  ptr_web_response->is_set = true; 
  ptr_web_response->file_name = file_name; // freed by caller
  status = extract_extension(file_name, &extension); cBYE(status);
  status = set_http_headers(extension, is_cache, 
      &(ptr_web_response->num_headers),
      &(ptr_web_response->header_key),
      &(ptr_web_response->header_val));
BYE:
  free_if_non_null(extension);
  return status;
}
