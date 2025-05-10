// similar to strcat but with files 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "isfile.h"
#include "rs_mmap.h"
#include "file_cat.h"

int
file_cat(
    const char * const dst,
    const char * const src
    )
{
  int status = 0;
  FILE *dfp = NULL; char *X = NULL; size_t nX = 0;
  char *xdst = NULL, *xsrc = NULL; 

  if ( dst == NULL ) { go_BYE(-1); }
  if ( src == NULL ) { go_BYE(-1); }
  if ( !isfile(dst) ) { go_BYE(-1); }
  if ( !isfile(src) ) { go_BYE(-1); }

  xdst = realpath(dst, NULL);
  xsrc = realpath(src, NULL);
  if ( strcmp(xdst, xsrc) == 0 ) { go_BYE(-1); }

  dfp = fopen(dst, "a");
  return_if_fopen_failed(dfp, dst, "a");
  status = rs_mmap(src, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { goto BYE; }
  fwrite(X, nX, 1, dfp);

BYE:
  free_if_non_null(xdst);
  free_if_non_null(xsrc);
  mcr_rs_munmap(X, nX); 
  fclose_if_non_null(dfp);
}
