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
#include "dist_j_k.h"
#include "ispc_dist_j_k.h"
#include "calc_dist.h"

// compute offset into distance array 
static inline size_t 
calc_offset(
    int j, 
    int k, 
    int nS,
    size_t nD
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
  // printf("%02d,%02d,%4ld \n", j, k, offset); 
  if ( offset > nD ) { WHEREAMI; return UINT_MAX; } // some bad number
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
  uint64_t nD = 0; // number of distances
  todo_t *todo = NULL; 
  uint16_t *D = NULL; 

  // open chromosomes for reading 
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status); // read-only
  // open distances for writing 
  status = rs_mmap(opfile, &Y, &nY, 1); cBYE(status); // writable 
  memset(Y, 0, nY); // not necessary *think*
  nD = (uint64_t)(nY / sizeof(uint16_t));  // number of "distances"
  if ( nD * sizeof(uint16_t) != nY ) { go_BYE(-1); } 
  D = (uint16_t *)Y;
  //----------------------------------------------------------------
  uint64_t x = (uint64_t)ceil((double)nM/2.0); 
  int nMprime = (int)multiple_n(x, REGISTER_SZ_BYTES);
  // 2*k*k + k*nM = cache size = 2^9
  int blksz = 2;  // TODO P1
  int max_todo = blksz * blksz;
  todo = malloc((size_t)max_todo * sizeof(todo_t));
  memset(todo, 0,  (size_t)max_todo * sizeof(todo_t));
  /* naive implementation 
  for ( int j = 0; j < nS; j++ ) { // row index
    for ( int k = j; k < nS; k++ ) {  // column index
      // calculate dist between j and k 
    }
  }
  */
  for ( int j = 0; j < nS; j += blksz ) { // row index
    for ( int k = j; k < nS; k += blksz ) {  // column index
      // We will compute distances between (j, j+1, ..) and (k, k+1, ...)
      int jlb = j;
      int jub = jlb + blksz; if ( jub > nS ) { jub = nS; } 
      int klb = k;
      int kub = klb + blksz; if ( kub > nS ) { kub = nS; } 

      if ( klb < jlb ) { go_BYE(-1); }

      // printf(" [%d, %d] -> [%d, %d ] \n", jlb, jub, klb, kub);
      // #pragma omp parallel for 
      int ll = 0; 
      for ( int jj = jlb; jj < jub; jj++ ) { 
        for ( int kk = klb; kk < kub; kk++ ) { 
          if ( kk <= jj ) { continue; } 
          // D[jj][kk] = 9999;
          size_t offset = calc_offset(jj, kk, nS, nD);
          todo[ll].j = jj;
          todo[ll].k = kk;
          todo[ll].o = offset;
          ll++;
        }
      }
      int num_todo = ll;
#pragma omp parallel for 
      for ( int i = 0; i < num_todo; i++ ) {
        size_t offset = todo[i].o;
        int jjj = todo[i].j;
        int kkk = todo[i].k;
        if ( D[offset] != 0 ) { go_BYE(-1); } 
#ifdef DEBUG
        uint16_t d1;
        status = c_dist_j_k((uint8_t *)X, jjj, kkk, nM, nMprime, &d1);
        cBYE(status);
#endif
        size_t bytes_per_sample = 64; // TODO P0
        uint8_t *Xj = (uint8_t *)X + (size_t)jjj*bytes_per_sample;
        uint8_t *Xk = (uint8_t *)X + (size_t)kkk*bytes_per_sample;
        uint16_t d2;
        ispc_dist_j_k(Xj, Xk, bytes_per_sample, &d2); 
#ifdef DEBUG
        if ( d1 != d2 ) { go_BYE(-1); }
#endif
        D[offset] = d2;
      }
    }
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
