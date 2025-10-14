#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "csp_info.h"
#include "csp_parse.h"
#include "csp_synthesize.h"
#include "str_as_file.h"
#include "q_macros.h"

int 
main(
    int argc, 
    char** argv
    ) 
{
  int status = 0;
  char *str = NULL; uint32_t len = 0;
  csp_info_t csp_info;
  memset(&csp_info, 0, sizeof(csp_info_t));

  if ( argc != 3 ) { go_BYE(-1); }
  const char * csp_file = argv[1]; 
  const char * out_csp_file = argv[2]; 
  status = csp_parse(csp_file, &csp_info); cBYE(status);
  status = csp_synthesize(&csp_info, &str, &len); cBYE(status);
  status = str_as_file(str, out_csp_file); cBYE(status);

  printf("SUCCESS\n");
BYE:
  free_if_non_null(str);
  // TODO free_csp(&csp_info);
  return status;
}
