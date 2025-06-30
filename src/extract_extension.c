#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "q_macros.h"
#include "extract_extension.h"

int 
extract_extension(
    const char * const X,
    char **ptr_ext
    )
{
  int status = 0;
  char *Y = NULL;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  size_t len = strlen(X);
  int dotpos = -1;
  for ( int i = len; i > 0; i-- ) { 
    if ( X[i] == '.' ) { dotpos = i; break; }
  }
  if ( dotpos <= 0 ) { go_BYE(-1); }
  len -= dotpos; 
  Y = malloc(len); memset(Y, 0, len);
  strcpy(Y, X+dotpos+1);

  *ptr_ext = Y;
BYE:
  return status;
}
#undef TEST
#ifdef TEST
int
main()
{
  int status = 0;
  char *X = "abc.txt";
  char *Y = NULL;
  status = extract_extension(X, &Y);
  if ( strcmp(Y, "txt") != 0 ) { go_BYE(-1); }
  free(Y);
  X = "abctxt";
  status = extract_extension(X, &Y); cBYE(status);
  if ( Y != NULL ) { go_BYE(-1); }
  X = ".txt";
  status = extract_extension(X, &Y); cBYE(status);
  if ( Y != NULL ) { go_BYE(-1); }
  printf("SUCCESS\n");
BYE:
  return status;
}
#endif
