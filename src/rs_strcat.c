//  returns new string created by concatenating 2 input strings
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "q_macros.h"
#include "rs_strcat.h"
int
rs_strcat(
    const char * const in1,
    const char * const in2,
    char **ptr_out
    )
{
  int status = 0;

  *ptr_out = NULL;
  char *out = NULL;
  if ( in1 == NULL ) { go_BYE(-1); }
  if ( in2 == NULL ) {  go_BYE(-1); }
  size_t len1 = strlen(in1);
  size_t len2 = strlen(in2);
  if ( len1 > SIZE_MAX - len2 - 1 ) { go_BYE(-1); }
  size_t len = len1+len2+1; // +1 for nullc
  out = malloc(len);
  if ( out == NULL ) { go_BYE(-1); }
  memcpy(out, in1, len1);
  memcpy(out + len1, in2, len2 + 1);
  *ptr_out = out;
BYE:
  return status;
}
