#include <math.h>
#include <string.h>
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

  // open chromosomes for reading 
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status); // read-only
  // open distances for writing 
  status = rs_mmap(opfile, &Y, &nY, 1); cBYE(status); // writable 
  memset(Y, 0, nY);
  //----------------------------------------------------------------
  int blksz = 2;  // TODO P1
  for ( int j = 0; j < nS; j += blksz ) { 
    for ( int k = j; k < nS; k += blksz ) { 
      // We will compute distances between (j, j+1, ..) and (k, k+1, ...)
      int jlb = j;
      int jub = jlb + blksz; if ( jub > nS ) { jub = nS; } 
      int klb = k;
      int kub = klb + blksz; if ( kub > nS ) { kub = nS; } 

      if ( klb < jlb ) { go_BYE(-1); }

      printf(" [%d, %d] -> [%d, %d ] \n", jlb, jub, klb, kub);
      // #pragma omp parallel for 
      for ( int jj = jlb; jj < jub; jj++ ) { 
        for ( int kk = klb; kk < kub; kk++ ) { 
          if ( kk <= jj ) { continue; } 
          // compute offset into distance array 
          size_t offset = 0; 
          if ( jj == 0 ) { 
            offset = 0;
          }
          else if ( jj == 1 ) { 
            offset = nS;
          }
          else {
            offset = (size_t)((jj*nS) - (jj*(jj-1))/ 2);
          }
          if ( jj == 0 ) { 
            offset += kk; 
          }
          else { 
            offset += kk - k; 
          }
          printf("%02d,%02d,%4ld \n", jj, kk, offset); 
          if ( offset > nY ) { go_BYE(-1); } 
          if ( Y[offset] != 0 ) { go_BYE(-1); }
          Y[offset] = 1;
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
    printf("=================================\n");
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
