#include <stdio.h>
#include "isdir.h"

int 
main(
    int argc, 
    char** argv
    ) 
{
  int status = 0;
  if ( argc != 2 ) { go_BYE(-1); } 
  const char *path = argv[1]; 
  printf("Input path = %s\n", path);
  if ( !isdir(path) ) { go_BYE(-1); } 
  printf("SUCCESS\n");
BYE:
  return status;
}
