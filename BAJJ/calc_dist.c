#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "q_macros.h"
#include "multiple.h"
#include "rs_mmap.h"
#include "consts.h"
#include "calc_dist.h"

// make distance matrix as a file which will be mmap'd
int
calc_dist(
    int nS, // number of samples 
    int nM, // number of markers
    const char * const infile, // input  file 
    const char * const opfile // output file 
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  char *Y = NULL; size_t nY = 0;

  int nMprime = (int)multiple_n((uint64_t)nM, REGISTER_SZ_BYTES); 
  int nSprime = (int)multiple_n((uint64_t)nS, REGISTER_SZ_BYTES); 

  status = rs_mmap(infile, &X, &nX, 0); cBYE(status); // read-only
  status = rs_mmap(opfile, &Y, &nY, 1); cBYE(status); // writable 
  //----------------------------------------------------------------
  int blksz = 2;  // TODO P1
  for ( int j = 0; j < nS; j++ ) {
    for ( int k = 0; k < nS; k++ ) { 
      // We will compute distances between (j, j+1, ..) and (k, k+1, ...)
      int jlb = j*blksz; 
      int jub = jlb + blksz; if ( jub > nS ) { jub = nS; } 

      int klb = k*blksz; 
      int kub = klb + blksz; if ( kub > nS ) { kub = nS; } 

      if ( jlb < klb ) { continue; }

      // #pragma omp parallel for 
      for ( int jj = jlb; jj < jub; jj++ ) { 
        for ( int kk = klb; kk < kub; kk++ ) { 
          printf("%02d,%02d \n", jj, kk); 
          /*
          int d = 0;
          for ( l = 0; l < nMprime; l++ ) { 
            if ( X[jj][d] ==  X[kk][d] ) {
              d++;
            }
          }
          D[jj][kk] += d;
          */
        }
      }
    }
  }

  //----------------------------------------------------------------

BYE:
  mcr_rs_munmap(X, nX);
  mcr_rs_munmap(Y, nY);
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
  int nM = NUM_THINGYS;
  status = calc_dist(nS, nM, "./_chromosome.bin", "./_dist.bin");
  cBYE(status);
BYE:
  return status;
}

#endif // UNIT_TEST
