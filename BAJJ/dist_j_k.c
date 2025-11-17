#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "q_macros.h"
#include "multiple.h"
#include "consts.h"
#include "dist_j_k.h"

int
c_dist_j_k(
    uint8_t *X, 
    int j, 
    int k, 
    int nM,
    int nMprime, // number of bytes allocated to a distance vector
    uint16_t *ptr_d
    )
{
  int status = 0;
  uint16_t d = 0;

  register int l = 0;
  register int maxi = (int)(ceil(nM/2.0));
  for ( int i = 0; i < nMprime; i++ ) { 
    uint8_t xj = X[j*nMprime+i];
    uint8_t xk = X[k*nMprime+i];
    if ( i > maxi ) {
#ifdef DEBUG
      if ( xj != 0 ) { go_BYE(-1); }
      if ( xk != 0 ) { go_BYE(-1); }
#endif
      break; 
    }
#ifdef DEBUG
    if ( xj == 0 ) { go_BYE(-1); }
    if ( xk == 0 ) { go_BYE(-1); }
#endif 
    uint8_t topj = xj >> 4;
    uint8_t topk = xk >> 4;
#ifdef DEBUG
    if ( ( topj < MINVAL ) || ( topj > MAXVAL ) ) { go_BYE(-1); }
    if ( ( topk < MINVAL ) || ( topk > MAXVAL ) ) { go_BYE(-1); }
#endif
    if ( topj == topk ) { d++; } 
    l++;
    if ( l >= nM ) { break; }

    uint8_t botj = xj & (uint8_t)0xF; 
    uint8_t botk = xk & (uint8_t)0xF; 
#ifdef DEBUG
    if ( ( botj < MINVAL ) || ( botj > MAXVAL ) ) { go_BYE(-1); }
    if ( ( botk < MINVAL ) || ( botk > MAXVAL ) ) { go_BYE(-1); }
#endif 
    if ( botj == botk ) { d++; } 
    l++;
  }
  *ptr_d = d;
  // printf("j/k/d = %2d %2d %3d \n", j, k, d);
BYE:
  return status;
}
