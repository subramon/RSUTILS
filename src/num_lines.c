#include "q_incs.h"
#include "rs_mmap.h"
#include "num_lines.h"
int
num_lines(
    const char * const infile,
    char *in_X,
    size_t in_nX
    )
{
  int status = 0;
  int nl = 0;
  char *X = NULL; size_t nX = 0;
  if ( infile == NULL ) { 
    if ( ( in_X == NULL ) || ( in_nX == 0 ) ) { go_BYE(-1); }
  }
  else {
    if ( ( in_X != NULL ) || ( in_nX != 0 ) ) { go_BYE(-1); }
  }
  if ( in_X != NULL ) { 
    X = in_X;
    nX = in_nX;
  }
  else {
    status = rs_mmap(infile, &X, &nX, 0); cBYE(status);
  }
  // CAUTION!!! CAUTION!!!
  // Makes very simplistic assumptions about CSV file e.g.,
  // newline terminates line and cannot be part of a value
  for ( size_t i = 0; i < nX; i++ ) { 
    if ( X[i] == '\n' ) { nl++; }
  }
BYE:
  if ( in_X == NULL ) { mcr_rs_munmap(X, nX); } 
  if ( status != 0 ) { return -1; } else { return nl; }
}
