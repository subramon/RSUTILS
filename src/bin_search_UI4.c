#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "q_macros.h"
#include "bin_search_UI4.h"


int
bin_search_UI4(
    const uint32_t * const X,
    uint32_t nX,
    uint32_t key,
    bool *ptr_found,
    uint32_t *ptr_pos
    )
{
  int status = 0;
  *ptr_found = false;
  *ptr_pos = 0; 
  uint32_t lb, ub, mid;

  if ( X == NULL ) go_BYE(-1);
  if ( nX <= 0 ) go_BYE(-1);
  //-- Handle degenerate case
  if ( nX == 1 ) { 
    if ( X[0] == key ) { 
      *ptr_pos = 0; 
      *ptr_found = true;
    }
    return status ; 
  }
  //-----------------------------------------------

  lb = 0; ub = nX;
  for ( ; lb < ub ; ) {  // TODO P2 Confirm that this is not <=
    mid = ( lb + ub )  / 2 ;
    if ( X[mid] == key ) { 
      *ptr_pos = mid;
      *ptr_found = true;
      break;
    }
    else if ( key < X[mid] ) { /* reduce ub */
      ub = mcr_min(ub - 1, mid);
    }
    else /* key > X[mid] */ { /* increase lb */
      lb = mcr_max(lb + 1, mid);
    }
  }
BYE:
  return status;
}
