//  returns new string created by concatenating 2 input strings
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "q_macros.h"
#include "rs_strcat.h"
char *
rs_strcat(
    const char * const in1,
    const char * const in2
    )
{
  char *out = NULL;
  if ( in1 == NULL ) { WHEREAMI; return NULL; }
  if ( in2 == NULL ) { WHEREAMI; return NULL; }
  size_t len1 = strlen(in1);
  size_t len2 = strlen(in2);
  size_t len = len1+len2+1; // +1 for nullc
  out = malloc(len); 
  if ( out == NULL ) { WHEREAMI; return NULL; }
  strcpy(out, in1);
  strcat(out, in2);
  return out;
}
