#include "q_incs.h"
#include "addr_as_hex_string.h"
// TODO IMPROVE hex() and unhex()
uint8_t
unhex(
    char x
   )
{
  switch ( x ) { 
    case '0' : return 0; 
    case '1' : return 1;
    case '2' : return 2;
    case '3' : return 3;
    case '4' : return 4;
    case '5' : return 5;
    case '6' : return 6;
    case '7' : return 7;
    case '8' : return 8;
    case '9' : return 9;
    case 'A' : return 0xA;
    case 'B' : return 0xB;
    case 'C' : return 0xC;
    case 'D' : return 0xD;
    case 'E' : return 0xE;
    case 'F' : return 0xF;
    default  : return UCHAR_MAX;
  }
}
char
hex(
    uint8_t x
   )
{
  switch ( x ) { 
    case 0 : return '0';
    case 1 : return '1';
    case 2 : return '2';
    case 3 : return '3';
    case 4 : return '4';
    case 5 : return '5';
    case 6 : return '6';
    case 7 : return '7';
    case 8 : return '8';
    case 9 : return '9';
    case 10 : return 'A';
    case 11 : return 'B';
    case 12 : return 'C';
    case 13 : return 'D';
    case 14 : return 'E';
    case 15 : return 'F';
    default : return '_';
  }
}
char *
addr_as_hex_string(
    void *x
    )
{
  if ( x == NULL ) { WHEREAMI; return NULL; }
  char *rslt = malloc(16+8); 
  memset(rslt, 0,  16+8); 
  uint64_t y = (uint64_t)x;
  for ( int i = 0; i < 16; i++ )  {
    uint64_t z = y & 0xF;
    char digit =  hex((uint8_t)z);
    rslt[i] = digit;
    y = y >> 4; 
  }
  return rslt;
}
uint64_t
hex_string_as_addr(
    const char * const str
    )
{
  if ( str == NULL ) { WHEREAMI; return 0; }
  uint64_t y = 0;
  for ( int i = 0; i < 16; i++ )  {
    char digit =  str[i];
    uint8_t h = unhex(digit);
    uint64_t z = h;
    z = z << (uint32_t)((4*i));
    y = y | z;
    // printf("%2d -->  %c %u -> %" PRIu64 "\n", i, digit, h, y);
  }
  return y;
}
