#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "bf16tofp32.h"

float 
bf16tofp32(
    uint16_t inval
    )
{
  float fval = 0;
  uint32_t ui32 = 0;
  uint32_t sign = inval & 0x8000; sign = sign >> 15;
  uint32_t frac = inval & 0x007F; frac = frac << 16;
  uint32_t exp  = inval & 0x7F80; exp  = exp  >> 7;
  /*
  int exponent = ( inval & 7600 ) ;
  printf("exponent = %d \n", exponent);
  exponent = ( inval & 7600 )  >> 10;
  printf("exponent = %d \n", exponent);
  */

  ui32 = ui32 | ( sign << 31); 
  ui32 = ui32 | ( exp  << 23); 
  ui32 = ui32 | ( frac << 0); 
  memcpy(&fval, &ui32, sizeof(uint32_t));
  
  return fval;
}

double 
bf16tofp64(
    uint16_t inval
    )
{
  float fval = bf16tofp32(inval);
  return (double)fval;
}
