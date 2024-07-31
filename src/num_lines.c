#include "q_incs.h"
#include "rs_mmap.h"
#include "num_lines.h"
int
num_lines(
    const char * const infile
    )
{
  int status = 0;
  int nl = 0;
  char *X = NULL; size_t nX = 0;
  if ( infile == NULL ) { go_BYE(-1); }
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status);
  // CAUTION!!! CAUTION!!!
  // Makes very simplistic assumptions about CSV file e.g.,
  // newline terminates line and cannot be part of a value
  for ( size_t i = 0; i < nX; i++ ) { 
    if ( X[i] == '\n' ) { nl++; }
  }
BYE:
  if ( X != NULL ) { munmap(X, nX); }
  if ( status < 0 ) { return status; } else { return nl; }
}
