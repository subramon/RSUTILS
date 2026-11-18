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
  if ( ( exp == 0) && (frac == 0 ) ) { return 0; } // zero
  if (exp == 0xFFu) { return (uint16_t)(ui32 >> 16); } // infinity or NaNs

  uint32_t ui16 = 0;
  ui16 = ui16 | (sign << 15); 
  ui16 = ui16 | (exp << 7);
  ui16 = ui16 | (frac >> 16);

  return (uint16_t)ui16;
}
