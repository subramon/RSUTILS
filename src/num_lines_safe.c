// num_lines is fast but makes some simplstic assumptions
// This is slower, needs more information but is stronger
#include "q_incs.h"
#include "rs_mmap.h"
#include "line_finder.h"
#include "num_lines_safe.h"

int
num_lines_safe(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    uint32_t max_width,
    uint32_t ncols,
    bool is_hdr,
    const char * const str_fld_sep,
    const char * const str_rec_sep,

    uint32_t *ptr_nrows
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  uint32_t *widths = NULL;
  bool *is_load = NULL;

  if ( max_width == 0 ) { max_width = 1023; } // default 
  // START: Check input arguments 
  // Provide either infile or in_X, in_nX but not both 
  if ( infile == NULL ) { 
    if ( ( in_X == NULL ) || ( in_nX == 0 ) ) { go_BYE(-1); }
  }
  else {
    if ( ( in_X != NULL ) || ( in_nX != 0 ) ) { go_BYE(-1); }
  }
  if ( str_fld_sep == NULL ) { go_BYE(-1); }
  if ( str_rec_sep == NULL ) { go_BYE(-1); }

  if ( strlen(str_fld_sep) != 1 ) { go_BYE(-1); }
  if ( strlen(str_rec_sep) != 1 ) { go_BYE(-1); }
  // STOP : Check input arguments 
  char fld_sep   = str_fld_sep[0];
  char rec_sep   = str_rec_sep[0];

  if ( infile != NULL ) { 
    status = rs_mmap(infile, &X, &nX, 0); cBYE(status);
  }
  else {
    X = in_X;
    nX = in_nX;
  }

  // allocate some stuff that line_finder needs
  widths = malloc(ncols * sizeof(uint32_t));
  for ( uint32_t i = 0; i < ncols; i++ ) { widths[i] = max_width; }
  is_load = malloc(ncols * sizeof(bool));
  for ( uint32_t i = 0; i < ncols; i++ ) { is_load[i] = false; }
  uint32_t nrows = 0;
  uint32_t eoln_idx = 0;
  char *max_X = X + nX;
  // handle the header line if any 
  if ( is_hdr ) {  // skip over first line 
    bool found_eoln = false;
    for ( uint64_t i = 0; i < nX; i++ ) { 
      if ( X[i] == '\n' ) { 
        X += i+1;
        nX -= i+1;
        found_eoln = true;
        break;
      }
    }
    if ( !found_eoln ) { go_BYE(-1); }
  }
  // --------------------------
  for ( ; ; ) { 
    status = line_finder( X, nX, fld_sep, rec_sep, ncols,
        widths, is_load, NULL, &eoln_idx);
    cBYE(status);
    if ( eoln_idx == 0 ) { go_BYE(-1); }
    nrows++; 
    X += eoln_idx; 
    nX -= eoln_idx; 
    if ( X > max_X ) { go_BYE(-1); }
    if ( X == max_X ) { 
      if ( nX != 0 ) { go_BYE(-1); } 
    break;
    }
  }
  *ptr_nrows = nrows; 
BYE:
  free_if_non_null(widths);
  free_if_non_null(is_load);
  return status;
}
