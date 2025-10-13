#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "q_macros.h"
#include "csp_info.h"

#include "rs_mmap.h"
#include "csp_parse.h"
int
csp_parse(
    const char * const csp_file,
    csp_info_t *ptr_csp_info

    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  size_t start, stop;

  status = rs_mmap(csp_file, &X, &nX, 0); cBYE(status);
  



BYE:
  mcr_rs_munmap(X, nX);
  return status;
}

