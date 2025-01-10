#include "q_macros.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include "line_finder.h"

#undef PRINT_CELLS
// If you want bslash or dquote in the value, then you MUST 
// start and stop the value with a dquote 
typedef enum  {
  S0,
  S1,
  S2,
  S3
} state_t;

static bool to_write(
    char **cells, 
    bool *is_load, 
    uint32_t col_idx
    )
{
  if ( ( cells != NULL ) && 
      ( ( is_load == NULL ) || 
        ( ( is_load != NULL ) && ( is_load[col_idx] == true ) ) ) ) {
    return true;
  }
  else {
    return false;
  }
}
static int rs_strcpy(
    char *dst,
    const char *const src,
    uint32_t start,
    uint32_t stop
    )
{
  int status = 0;
  if ( dst == NULL ) { go_BYE(-1); }
  if ( src == NULL ) { go_BYE(-1); }
  if ( start > stop ) { // TODO > or >= ??
    go_BYE(-1); 
  } 
  // clean out dst
  for ( char *cptr = dst; *cptr != '\0'; cptr++ ) { 
    *cptr = '\0';
  }
  uint32_t didx = 0;
  for ( uint32_t sidx = start; sidx < stop; sidx++, didx++ ) { 
    if ( src[sidx] == '\\' ) { 
      sidx++;
    }
    dst[didx] = src[sidx];
  }
BYE:
  return status;
}

int
line_finder(
    const char * const X, // [nX] input 
    uint64_t nX,
    int fld_sep,
    int rec_sep,
    uint32_t ncols, // number of columns 
    uint32_t *max_width, // [ncols] can be null 
    // Note that max_width needs one spot for nullc
    bool *is_load, // [ncols] can be null 
    char **cells, // [ncols][max_width[i]] can be null
    uint32_t *ptr_eoln_idx
    )
{
  int status = 0;
  if ( X == NULL ) { go_BYE(-1); } 
  if ( nX == 0 ) { go_BYE(-1); } 
  if ( ncols == 0 ) { go_BYE(-1); } 
  uint32_t col_idx = 0;
  uint32_t start_idx, stop_idx;
  char dquote = '"';
  char bslash = '\\';
  state_t state = S0;
  int num_bslash = 0;
  *ptr_eoln_idx = 0;
  bool eoln_found = false;
  for ( uint64_t idx = 0; eoln_found == false; idx++ ) {
    if ( idx == nX ) { 
      go_BYE(-1); } 
    char c = X[idx];
    switch ( state ) {
      case S0 : 
        if ( ( col_idx == ncols-1) && ( c == rec_sep ) ) {
#ifdef PRINT_CELLS
          printf("Identified %dth cell = []\n", col_idx);
#endif
          *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
          eoln_found = true;
          break;
        }
        if ( ( col_idx <  ncols-1) && ( c == fld_sep ) ) {
          if ( to_write(cells, is_load, col_idx) ) {
            cells[col_idx][0] = '\0';
          }
#ifdef PRINT_CELLS
          printf("Identified %dth cell = []\n", col_idx);// empty cell 
#endif
          col_idx++;
          break;
        }
        if ( c == dquote ) {
          state = S1;
          num_bslash = 0;
          start_idx = idx+1; //+1 skips over dquote
        }
        else if ( c == bslash ) {
          bool null_cell = false;
          // START: Logic to handle \N as being a null value 

          if ( idx+2 < nX ) {
            if ( X[idx+1] == 'N' ) {
              if ( ( X[idx+2] == fld_sep ) || ( X[idx+2] == rec_sep ) ) {
                null_cell = true;
              }
            }
          }
          if ( null_cell ) { 
            if ( is_load[col_idx] ) { 
              cells[col_idx][0] = '\0';
            }
            col_idx++;
            break;
          }
          else {
            go_BYE(-1);
          }
        }
        else {
          state = S2;
          start_idx = idx;
        }
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
              if ( col_idx > ncols ) { go_BYE(-1); }
              // Note that max_width needs one spot for nullc
              if ( width >= max_width[col_idx] ) { go_BYE(-1); }
            }
#ifdef PRINT_CELLS
               printf("Identified %dth cell = [", col_idx);
               for ( uint32_t j = start_idx; j < stop_idx; j++ ) { 
               printf("%c", X[j]);
               }
               printf("]\n");
#endif
            if ( to_write(cells, is_load, col_idx) ) {
              status = rs_strcpy(cells[col_idx], X, start_idx, stop_idx);
              cBYE(status);
            }
            if ( ( col_idx == ncols) && ( c == rec_sep ) ) {
              *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
              eoln_found = true;
              break;
            }
            col_idx++;  // get ready for next cell
            state = S0;
            idx++; // jump over dquote
            if ( idx == nX ) { go_BYE(-1); }
            c = X[idx];
            if ( ( col_idx <  ncols) && ( c != fld_sep ) ) { go_BYE(-1); }
            if ( ( col_idx == ncols) && ( c != rec_sep ) ) { go_BYE(-1); }
            if ( ( col_idx == ncols) && ( c == rec_sep ) ) {
              *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
              eoln_found = true;
              break;
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
            ( ( col_idx <  ncols-1) && ( c == fld_sep ) ) || 
            ( ( col_idx == ncols-1) && ( c == rec_sep ) ) ) {
          stop_idx = idx;
          if ( max_width != NULL ) { 
            uint32_t width = stop_idx - start_idx;
            if ( col_idx >= ncols ) { go_BYE(-1); }
            if ( ( width > max_width[col_idx] ) && 
                ( max_width[col_idx] > 0 ) ) {
              go_BYE(-1); 
            }
          }
#ifdef PRINT_CELLS
             printf("Identified %dth cell = [", col_idx);
             for ( uint32_t j = start_idx; j < stop_idx; j++ ) { 
             printf("%c", X[j]);
             }
             printf("]\n");
#endif
          if ( to_write(cells, is_load, col_idx) ) {
            status = rs_strcpy(cells[col_idx], X, start_idx, stop_idx);
            cBYE(status);
          }
          if ( ( col_idx == ncols) && ( c == rec_sep ) ) {
            *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
            eoln_found = true;
            break;
          }
          col_idx++; // get ready for next cell
          state = S0;
          if ( ( col_idx <  ncols) && ( c != fld_sep ) ) { go_BYE(-1); }
          if ( ( col_idx == ncols) && ( c != rec_sep ) ) { go_BYE(-1); }
          if ( ( col_idx == ncols) && ( c == rec_sep ) ) {
            *ptr_eoln_idx = idx+1; // +1 because ub is exclusive
            eoln_found = true;
            break;
          }
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
