#include "q_incs.h"
#include "q_macros.h"
#include "file_exists.h"

int 
main(
    int argc, 
    char** argv
    ) 
{
  int status = 0;
  if ( argc != 2 ) { go_BYE(-1); } 
  const char * file_name = argv[1];
  status = file_exists(file_name); cBYE(status);
  printf("SUCCESS\n");
BYE:
  return 0;
}
