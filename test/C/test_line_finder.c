#include "q_macros.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include "line_finder.h"

int
t1(
    uint32_t nC,
    uint32_t *max_width,
    bool *is_load,
    char **cells
  )
{
  int status = 0;
  char *X = "abc,defg,hijkl\n\"123\",4567,\"8910\"\n,xxxx,\n";
  uint64_t nX = strlen(X);


  uint32_t eoln_idx; 
  status = line_finder(X, nX, 44, 10, nC, max_width, is_load, cells, &eoln_idx);
  cBYE(status);
  if ( eoln_idx != 15 ) { go_BYE(-1); }
  // printf("eoln_idx = %" PRIu32 "\n", eoln_idx);
  X += eoln_idx;
  nX -= eoln_idx;
  status = line_finder(X, nX, 44, 10, nC, max_width, is_load, cells, &eoln_idx);
  cBYE(status);
  if ( eoln_idx != 18 ) { go_BYE(-1); }
  // printf("eoln_idx = %" PRIu32 "\n", eoln_idx);
  X += eoln_idx;
  nX -= eoln_idx;
  status = line_finder(X, nX, 44, 10, nC, max_width, is_load, cells, &eoln_idx);
  cBYE(status);
  if ( eoln_idx != 7 ) { go_BYE(-1); }
  // printf("eoln_idx = %" PRIu32 "\n", eoln_idx);
  printf("Completed test t1\n");
BYE:
  return status;
}

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  uint32_t nC = 3;
  int nW = 8;
  uint32_t max_width[nC];
  bool is_load[nC];
  char **cells = NULL;
  cells = malloc(nC * sizeof(char *));
  for ( uint32_t i = 0; i< nC; i++ ) { 
    cells[i] = malloc(nW * sizeof(char));
    memset(cells[i], 0, nW * sizeof(char));
  }


  if ( argc != 1 ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < nC; i++ ) { 
    max_width[i] = nW;
    memset(cells[i], 0, nW);
    is_load[i] = true;
  }
  status = t1(nC, NULL, NULL, NULL); cBYE(status);

  status = t1(nC, max_width, is_load, cells); cBYE(status);
  printf("SUCCESS. Completed %s \n", argv[0]);
BYE:
  return status;
}
