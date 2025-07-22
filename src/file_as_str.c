#include "q_incs.h"
#include "rs_mmap.h"
#include "file_as_str.h"
//START_FUNC_DECL
int
file_as_str(
    const char * const infile,
    char **ptr_str,
    size_t *ptr_len
    )
//STOP_FUNC_DECL
{
  int status = 0;
  *ptr_str = NULL;
  char *X = NULL; size_t nX = 0;
  char * Y = NULL;
  if ( infile == NULL ) { go_BYE(-1); }
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status);
  Y = malloc(nX+1);
  return_if_malloc_failed(Y);
  memcpy(Y, X, nX);
  *ptr_len = nX;
  *ptr_str = Y;
  Y[nX] = '\0';
BYE:
  if ( X != NULL ) { munmap(X, nX); } 
  return status;
}
