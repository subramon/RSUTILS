#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "q_macros.h"
#include "consts.h"
#include "multiple.h"
#include "make_data.h"

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
  int nMprime = (int)multiple_n((uint64_t)nM, REGISTER_SZ_BYTES); 
  fp = fopen(opfile, "wb");
  return_if_fopen_failed(fp, opfile, "wb");
  if ( seed == 0 ) { seed = (unsigned int)time(NULL); } 
  srandom(seed); 
  for ( int i = 0; i < nS; i++ ) { 
    for ( int j = 0; j < nM; j += 2 ) { 
      uint16_t top = (uint16_t)(MINVAL + (random() % (MAXVAL+1)));
      uint16_t bot = (uint16_t)(MINVAL + (random() % (MAXVAL+1)));
      uint8_t out = (uint8_t)( ( top << 4 ) | bot);
      fwrite(&out, sizeof(uint8_t), 1, fp);
    }
    // pad with zeroes
    // If nM was odd that means we wrote one extra above. 
    if ( ( ( nM / 2 ) * 2 ) != nM ) { 
      nM = nM+1; 
    }
    for ( int j = nM; j < nMprime; j += 2 ) { 
      uint8_t x = 0; 
      fwrite(&x, sizeof(uint8_t), 1, fp);
    }
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
