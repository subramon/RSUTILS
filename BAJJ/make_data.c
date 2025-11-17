#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "q_macros.h"
#include "consts.h"
#include "multiple.h"
#include "make_data.h"

// each sample has a multiple of 64 bytes
// marker takes on values 1, 2, ... 7
int
make_data(
    int nS, // number of samples 
    int nM, // number of markers
    unsigned int seed,
    const char * const opfile // output file 
    )
{
  int status = 0;
  FILE *fp = NULL;
  fp = fopen(opfile, "wb");
  return_if_fopen_failed(fp, opfile, "wb");
  if ( seed == 0 ) { seed = (unsigned int)time(NULL); } 
  srandom(seed); 
  for ( int i = 0; i < nS; i++ ) {
    int num_bytes_written = 0;
    for ( int j = 0; j < nM; ) {
      uint16_t top = (uint16_t)(MINVAL + (random() % (MAXVAL)));
      if (( top < MINVAL ) || ( top > MAXVAL )) { go_BYE(-1); }
      j++;
      uint16_t bot = 0;
      if ( j < nM ) { 
        bot = (uint16_t)(MINVAL + (random() % (MAXVAL)));
        if (( bot < MINVAL ) || ( bot > MAXVAL )) { go_BYE(-1); }
      }
      j++;
      uint8_t out = (uint8_t)( ( top << 4 ) | bot);
      fwrite(&out, sizeof(uint8_t), 1, fp);
      num_bytes_written++; 
    }
    // pad with zeroes
    int ub =  ( num_bytes_written / REGISTER_SZ_BYTES ) * REGISTER_SZ_BYTES;
    if ( ub != num_bytes_written ) { ub += REGISTER_SZ_BYTES; }
    for ( int j = num_bytes_written; j < ub; j++ ) { 
      uint8_t x = 0; 
      fwrite(&x, sizeof(uint8_t), 1, fp);
      num_bytes_written++;
    }
    // printf("i = %d, num_bytes_written = %d \n", i, num_bytes_written);
  }
  
BYE:
  fclose_if_non_null(fp); 
  return status;
}

#define UNIT_TEST
#ifdef UNIT_TEST
int
main(
    void
    )
{
  int status = 0;
  unsigned int seed = 1234;
  int nS = NUM_SAMPLES;
  int nM = NUM_THINGYS;
  status = make_data(nS, nM, seed, "./_chromosome.bin");
  cBYE(status);
BYE:
  return status;
}

#endif // UNIT_TEST
