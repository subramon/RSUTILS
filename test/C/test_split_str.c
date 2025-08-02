#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "free_2d_array.h"
#include "split_str.h"
#include "q_macros.h"

int 
main(
    int argc, 
    char** argv
    ) 
{
  int status = 0;
  char **outs = NULL; uint32_t n_outs = 0;
  char *test1 = "abc;def;ghi";
  char *test2 = ";;";
  char *test3 = "ABC;;;DEF";

  status = split_str(test1, ";", &outs, &n_outs); 
  cBYE(status);
  if ( n_outs != 3 ) { go_BYE(-1); }
  if ( strcmp(outs[0], "abc") != 0 ) { go_BYE(-1); }
  if ( strcmp(outs[1], "def") != 0 ) { go_BYE(-1); }
  if ( strcmp(outs[2], "ghi") != 0 ) { go_BYE(-1); }
  free_2d_array(&outs, n_outs);

  status = split_str(test2, ";", &outs, &n_outs); 
  cBYE(status);
  if ( n_outs != 3 ) { go_BYE(-1); }
  if ( *outs[0] != '\0' ) { go_BYE(-1); }
  if ( *outs[1] != '\0' ) { go_BYE(-1); }
  if ( *outs[2] != '\0' ) { go_BYE(-1); }
  free_2d_array(&outs, n_outs);

  status = split_str(test3, ";", &outs, &n_outs); 
  cBYE(status);
  if ( n_outs != 4 ) { go_BYE(-1); }
  if ( strcmp(outs[0], "ABC") != 0 ) { go_BYE(-1); }
  if ( *outs[1] != '\0' ) { go_BYE(-1); }
  if ( *outs[2] != '\0' ) { go_BYE(-1); }
  if ( strcmp(outs[3], "DEF") != 0 ) { go_BYE(-1); }
  free_2d_array(&outs, n_outs);



  printf("SUCCESS\n");
BYE:
  return status;
}
