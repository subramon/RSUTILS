#include <string.h>
#include "fp32tobf16.h"

uint16_t
fp32tobf16(
    float x
    )
{
  uint32_t ui32; memcpy(&ui32, &x, sizeof(float));
  
  uint32_t sign = ui32 & 0x80000000u;
  sign = sign >> 31;
  uint32_t exp  = ui32 & 0x7F800000u;
  exp = exp >> 23;
  uint32_t frac = ui32 & 0x007FFFFFu;
  if ( ( exp == 0) && (frac == 0 ) ) { return x; } // zero
  if (exp == 0x7F800000u) { return x; } // infinity or Nans

  uint16_t ui16 = 0;
  ui16 = ui16 | (sign << 15); 
  ui16 = ui16 | (exp << 7);
  ui16 = ui16 | (frac >> 16);

  return ui16;
}
