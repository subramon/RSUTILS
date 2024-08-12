#include "q_macros.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

// If you want bslash or dquote in the value, then you MUST 
// start and stop the value with a dquote 
typedef enum  {
  S0,
  S1,
  S2,
  S3
} state_t;

int
line_finder(
    const char * const X, // [nX] input 
    uint64_t nX,
    int fld_sep,
    int rec_sep,
    uint32_t ncol, // number of columns 
    uint32_t *max_width, // [ncol] can be null 
    uint32_t *ptr_eoln_idx
    )
{
  int status = 0;
  if ( X == NULL ) { go_BYE(-1); } 
  if ( nX == 0 ) { go_BYE(-1); } 
  if ( ncol == 0 ) { go_BYE(-1); } 
  uint32_t ncol_seen = 0; 
  int col_idx = 0;
  uint32_t start_idx, stop_idx;
  char dquote = '"';
  char bslash = '\\';
  state_t state = S0;
  int num_bslash = 0;
  *ptr_eoln_idx = 0;
  bool eoln_found = false;
  for ( uint64_t idx = 0; eoln_found == false; idx++ ) {
    if ( idx == nX ) { go_BYE(-1); } 
    char c = X[idx];
    switch ( state ) {
      case S0 : 
        if ( c == dquote ) {
          state = S1;
          num_bslash = 0;
          start_idx = idx+1; //+1 skips over dquote
        }
        else if ( c == bslash ) {
          go_BYE(-1);
        }
        else {
          state = S2;
          start_idx = idx;
        }
        col_idx++; // just started a new cell 
        break;
      case S1 : // inside a dquote demarcated cell 
        {
          if ( c == bslash ) {
            idx++; if ( idx == nX ) { go_BYE(-1); }
            c = X[idx]; 
            if ( ( c == bslash ) || ( c == dquote ) ) {
              num_bslash++;
            }
            else {
              go_BYE(-1);
            }
          }
          else if ( c == dquote ) { // end of cell 
            stop_idx = idx;
            if ( max_width != NULL ) { 
              uint32_t width = (stop_idx - start_idx) - num_bslash;
              if ( col_idx >= ncol ) { go_BYE(-1); }
              if ( width > max_width[col_idx] ) { go_BYE(-1); }
            }
            printf("Identified %dth cell = [", col_idx);
            for ( uint32_t j = start_idx; j < stop_idx; j++ ) { 
              printf("%c", X[j]);
            }
            printf("]\n");
            if ( ( col_idx == ncol) && ( c == rec_sep ) ) {
              *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
              eoln_found = true;
              break;
            }
            state = S0;
            idx++; // jump over dquote
            if ( idx == nX ) { go_BYE(-1); }
            c = X[idx];
            if ( ( col_idx <  ncol) && ( c != fld_sep ) ) { go_BYE(-1); }
            if ( ( col_idx == ncol) && ( c != rec_sep ) ) { go_BYE(-1); }
            if ( ( col_idx == ncol) && ( c == rec_sep ) ) {
              *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
              eoln_found = true;
              break;
            }
            else {
              state = S0;
            }
          }
          else {
            // keep going 
          }

        }
        break;
      case S2 : // inside a regular cell 
        if ( ( c == dquote ) || ( c == bslash ) ) {
          go_BYE(-1);
        }
        else if ( 
            ( ( col_idx <  ncol) && ( c == fld_sep ) ) || 
            ( ( col_idx == ncol) && ( c == rec_sep ) ) ) {
          stop_idx = idx;
          if ( max_width != NULL ) { 
            uint32_t width = stop_idx - start_idx;
            if ( col_idx >= ncol ) { go_BYE(-1); }
            if ( width > max_width[col_idx] ) { go_BYE(-1); }
          }
          printf("Identified %dth cell = [", col_idx);
          for ( uint32_t j = start_idx; j < stop_idx; j++ ) { 
            printf("%c", X[j]);
          }
          printf("]\n");
          if ( ( col_idx == ncol) && ( c == rec_sep ) ) {
            *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
            eoln_found = true;
            break;
          }
          state = S0;
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  if ( *ptr_eoln_idx == 0 ) { go_BYE(-1); }
BYE:
  return status;
}
