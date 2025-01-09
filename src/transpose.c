#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/* the reason for col_width and col_offset is as follows
 * Out input data is organized in terms of columns
 * Our output data is organized in terms of rows, where each row
 * is a struct of some type. Consider a struct of 
 * typedef struct _foo_t {
 * int8_t x;
 * int16_t y;
 * int32_t z;
 * } foo_t;
 C does NOT gaurantee that 
 sizeof(foo_t) == sizeof(int8_t) + sizeof(int16_t) + sizeof(int32_t)
So, we need to know where each col is located in the struct.
We can assert that col_offset[0] == 0
But, col_offset[1] could be 1 or 2 or 4
We can't calculate col_offset from col_width alone 
 * */

//START_FOR_CDEF
int
transpose(
    char ** cols, // [n_cols] 
    uint32_t *col_width,
    uint32_t *col_offset,
    uint32_t n_cols,
    uint32_t n_rows,
    char *out // [n_rows] 
    )
//STOP_FOR_CDEF
{
  int status = 0;
  for ( uint32_t r = 0; r < n_rows; r++ ) { 
    uint32_t cum_col_offset = 0;
    for ( uint32_t c = 0; c < n_cols; c++ ) { 
      char *dst = cols[c] + (r*col_width[c]);
      char *src = out + (r*sum_col_width) + cum_col_offset;
      memcpy(dst, src, col_width[c]);
      cum_col_offset += col_offset[c];
    }
  }
BYE:
  return status;
}
