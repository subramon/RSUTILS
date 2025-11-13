#include <stdio.h>
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

  uint64_t nSprime = (uint64_t)multiple_n((uint64_t)nS, REGISTER_SZ_BYTES); 
  // nD = n-1 + n-2 + ... 1 = n*(n-1)/2
  uint64_t num_dists = ((nSprime * (nSprime-1)) / 2); 
#ifdef DEBUG
  uint64_t chk = 0;
  for ( int i = nSprime -1 ; i >= 0; i-- ) { 
    chk += i;
  }
  if ( chk != num_dists ) { go_BYE(-1);
  }
#endif

  size_t fsz  = (size_t)(sizeof(uint8_t) * num_dists);
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
  status = make_dist(nS, "./_dist.bin");
  cBYE(status);
BYE:
  return status;
}

#endif // UNIT_TEST
