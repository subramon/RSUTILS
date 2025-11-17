#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "q_macros.h"
#include "multiple.h"
#include "mk_file.h"
#include "consts.h"
#include "make_dist.h"

// make distance matrix as a file which will be mmap'd
int
make_dist(
    int nS, // number of samples 
    const char * const opfile // output file 
    )
{
  int status = 0;
  FILE *fp = NULL;

  fp = fopen(opfile, "wb");
  return_if_fopen_failed(fp, opfile, "wb");

  // nD = 1 + 2 + ... n - a bit more than we need because (i,i) not needed
  uint64_t nD = (((uint64_t)nS * (uint64_t)(nS+1)) / 2); 
#ifdef DEBUG
  uint64_t chk = 0;
  for ( int i = 1; i <= nS; i++ ) { 
    chk += (uint64_t)i;
  }
  if ( chk != nD ) { go_BYE(-1);
  }
  printf("nS = %d, nD = %" PRIu64 "\n", nS, nD);
#endif

  // NOTE: that each entry of distance array is uint16_t
  size_t fsz  = (size_t)(sizeof(uint16_t) * nD);
  status = mk_file(NULL, "_dist.bin", fsz); cBYE(status);
  
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
  int nS = NUM_SAMPLES;
  status = make_dist(nS, "./_dist.bin"); cBYE(status);
  printf("Created empty file for distance array \n"); 
BYE:
  return status;
}

#endif // UNIT_TEST
