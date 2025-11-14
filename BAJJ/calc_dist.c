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

// compute offset into distance array 
static inline size_t 
calc_offset(
    int j, 
    int k, 
    int nS,
    size_t nY
    )
{
  size_t offset = 0; 
  if ( j == 0 ) { 
    offset = 0;
  }
  else if ( j == 1 ) { 
    offset = (size_t)nS;
  }
  else {
    offset = (size_t)((j*nS) - (j*(j-1))/ 2);
  }
  if ( j == 0 ) { 
    offset += (size_t)k; 
  }
  else { 
    offset += (size_t)(k - j); 
  }
  printf("%02d,%02d,%4ld \n", j, k, offset); 
  if ( offset > nY ) { WHEREAMI; return UINT_MAX; } // some bad number
  return offset;
}
// make distance matrix as a file which will be mmap'd
typedef struct _todo_t { 
    int j;
    int k;
    size_t o;
} todo_t;
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
  todo_t *todo = NULL; 

  // open chromosomes for reading 
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status); // read-only
  // open distances for writing 
  status = rs_mmap(opfile, &Y, &nY, 1); cBYE(status); // writable 
  memset(Y, 0, nY);
  //----------------------------------------------------------------
  int ctr = 1; 
  int blksz = 2;  // TODO P1
  int max_todo = blksz * blksz;
  todo = malloc((size_t)max_todo * sizeof(todo_t));
  memset(todo, 0,  (size_t)max_todo * sizeof(todo_t));
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
      int ll = 0; 
      for ( int jj = jlb; jj < jub; jj++ ) { 
        for ( int kk = klb; kk < kub; kk++ ) { 
          if ( kk <= jj ) { continue; } 
          size_t offset = calc_offset(jj, kk, nS, nY);
          todo[ll].j = jj;
          todo[ll].k = kk;
          todo[ll].o = offset;
          ll++;
        }
      }
      int num_todo = ll;
      for ( int i = 0; i < num_todo; i++ ) { 
        size_t offset = todo[i].o;
        if ( Y[offset] != 0 ) { go_BYE(-1); } // TODO FAKE 
        Y[offset] = 1;
      }

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
    printf("=================================\n");
  }

  //----------------------------------------------------------------

BYE:
  free_if_non_null(todo);
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
