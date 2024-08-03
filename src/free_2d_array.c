#include "q_incs.h"
#include "free_2d_array.h"

void
free_2d_array(
    char ***ptr_X,
    uint32_t n
    )
{
  char **X = *ptr_X;
  if ( X == NULL ) { return; }
  for ( uint32_t i = 0; i < n; i++ ) {
    free_if_non_null(X[i]);
  }
  free_if_non_null(X);
  *ptr_X = NULL;
}
