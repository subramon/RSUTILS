#include <stdbool.h>
#include <stdint.h>
#include "multiple.h"
bool 
is_multiple_n(
    uint64_t x,
    uint32_t n
    )
{
  if ( x == 0 ) { return false; }
  if ( ( ( x / n ) * n ) != x ) { return false; } else { return true; }
}

uint64_t 
multiple_n(
    uint64_t x,
    uint32_t n
    )
{
  if ( n == 8 )  {
  uint32_t m = (x >> 3) << 3;
  if ( m != x ) { m += n; }
  return m;
  }
  else {
  uint32_t m = (x / n) * n;
  if ( m != x ) { m += n; }
  return m;
  }
}
