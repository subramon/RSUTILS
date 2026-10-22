#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"
#include "sql_quote.h"
int
sql_quote(
    const char * const in_str,
    char **ptr_out_str
    )
{
  int status = 0;
  *ptr_out_str = NULL;
  char *out_str = NULL;
  if ( in_str == NULL ) { go_BYE(-1); }
  uint32_t num_sq = 0;
  size_t in_len = strlen(in_str);
  for ( const char *cptr = in_str; *cptr != '\0'; cptr++ ) { 
    if ( *cptr == '\'' ) { 
      num_sq++;
    }
  }
  size_t out_len = in_len + num_sq + 1; 
  out_str = malloc(out_len * sizeof(char));
  memset(out_str, 0, out_len);
  
  char *out_ptr = out_str;
  for ( const char *cptr = in_str; *cptr != '\0'; cptr++ ) { 
    if ( *cptr == '\'' ) { 
      *out_ptr++ = '\''; // extra squote 
    }
    *out_ptr++ = *cptr; 
  }
  *ptr_out_str = out_str; 
BYE:
  return status;
}

#ifdef UNIT_TEST
int
main()
{
  int status = 0;
  const char *X = "abc'def'g";
  char *Y = NULL;
  status =  sql_quote(X, &Y); cBYE(status);
  printf("X = %s \n", X);
  printf("Y = %s \n", Y);
BYE:
  free_if_non_null(Y);
  return status;
}
#endif
