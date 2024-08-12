#include "q_macros.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include "line_finder.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *X = "abc,defg,hijkl\n\"123\",4567,\"8910\"\n";
  uint64_t nX = strlen(X);

  if ( argc != 1 ) { go_BYE(-1); }

  uint32_t eoln_idx; 
  status = line_finder(X, nX, 44, 10, 3, NULL, &eoln_idx);
  cBYE(status);
  printf("eoln_idx = %" PRIu32 "\n", eoln_idx);
  X += eoln_idx;
  nX -= eoln_idx;
  status = line_finder(X, nX, 44, 10, 3, NULL, &eoln_idx);
  cBYE(status);
  printf("eoln_idx = %" PRIu32 "\n", eoln_idx);
  printf("SUCCESS. Completed %s \n", argv[0]);
BYE:
  return status;
}
