//START_INCLUDES
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"
//STOP_INCLUDES
#include "BL_to_txt.h"
//START_FUNC_DECL
int
BL_to_txt(
    bool val,
    char * X,
    size_t nX
    )
//STOP_FUNC_DECL
{
  int status = 0;
  if ( X == NULL ) { go_BYE(-1); }
  if ( nX < 6 ) { go_BYE(-1); }
  memset(X, '\0', nX);
  int nw = snprintf(X, nX, "%s", val ? "true" : "false");
  if ( ( nw < 0 ) || ( (size_t)nw >= nX ) ) { go_BYE(-1); }
BYE:
  return status;
}
