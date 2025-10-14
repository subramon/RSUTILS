#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "csp_info.h"
#include "csp_parse.h"
#include "q_macros.h"

int 
main(
    int argc, 
    char** argv
    ) 
{
  int status = 0;
  csp_info_t csp_info;
  memset(&csp_info, 0, sizeof(csp_info_t));

  if ( argc != 2 ) { go_BYE(-1); }
  const char * csp_file = argv[1]; 
  status = csp_parse(csp_file, &csp_info); cBYE(status);
  printf("SUCCESS\n");
BYE:
  return status;
}
