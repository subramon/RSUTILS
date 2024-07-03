#include "txt_to_I8.h"

//START_FUNC_DECL
int
txt_to_I8(
      const char * const X,
      int64_t *ptr_out
      )
//STOP_FUNC_DECL
{
  int status = 0;
  char *endptr = NULL;
  // not thread safe TODO P3 errno = 0;
  int64_t out = 0;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  out = strtoll(X, &endptr, 10);
  if ( ( endptr != NULL ) && ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { go_BYE(-1); }
  // not thread safe TODO P3 
  // if ( ( errno == ERANGE ) || ( errno == EINVAL ) ) { go_BYE(-1); } 
  if (endptr == X) { go_BYE(-1); }

  *ptr_out = (int64_t)out;
BYE:
  if ( status < 0 ) { 
    fprintf(stderr, "Could not convert [%s] to I8\n", X);
  }
  return status ;
}
