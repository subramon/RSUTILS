//  returns new string created by concatenating 2 input strings
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
  size_t len = len1+len2+1; // +1 for nullc
  out = malloc(len); 
  if ( out == NULL ) { go_BYE(-1); }
  strcpy(out, in1);
  strcat(out, in2);
  *ptr_out = out;
BYE:
  return status;
}
