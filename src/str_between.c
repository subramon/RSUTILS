#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "q_macros.h"
#include "str_between.h"

int
str_between(
    char * const in_str,
    char * const start,
    char * const stop,
    char **ptr_val
    )
// STOP FUNC DECL
{
  int status = 0;
  char *cptr1, *cptr2, *cptr3;
  char *val = NULL;

  if ( in_str == NULL ) { go_BYE(-1); }
  if ( ( start == NULL ) || ( *start == '\0' ) ) { go_BYE(-1); }
  if ( ( stop == NULL ) || ( *stop == '\0' ) ) { go_BYE(-1); }
  *ptr_val = NULL; 

  // Look for start delimiter 
  cptr1 = strstr(in_str, start);
  // If no match found, then quit 
  if ( cptr1 == NULL ) { return status; } 
  // advance ahead of start delimiter 
  cptr2 = cptr1 + strlen(start); 
  // Look for stop delimiter 
  cptr3 = strstr(cptr2, stop);
  // If no match found, then quit 
  if ( cptr3 == NULL ) { return status; }
  size_t len = cptr3 - cptr2; 
  val = malloc(len+1); 
  memset(val, 0,  len+1); 
  strncpy(val, cptr2, len); 
  *ptr_val = val; 
BYE:
  return status ;
}
