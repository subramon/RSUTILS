#include <sys/stat.h>
#include <stdio.h>
#include "q_macros.h"
#include "mkdir.h"

int 
mkdir(
    const char * const dir_name,
    mode_t mode
    )
{
  int status = 0;

  status = mkdir(dir_name, mode); 
  if ( status != 0 ) { 
    fprintf(stderr, "Unable to make directory %s \n", dir_name);
  }
  cBYE(status);
BYE:
  return status;
}
