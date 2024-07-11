#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "q_macros.h"
#include "rand_file_name.h"
#include "get_file_size.h"
#include "stretch.h"

#define N 4 
int
main(
    void
    ) 
{ 
  int status = 0;
  int len_file_name = 64;
  char file_name[len_file_name]; memset(file_name, 0, len_file_name); 
  FILE *fp = NULL;

  // make some dummy data 
  int Y[N];
  for ( int i = 0;  i < N; i++ ) { Y[i] = i+1; }
  // write it to a file 
  status = rand_file_name(file_name, len_file_name-1); cBYE(status);
  printf("file_name = %s \n", file_name); 
  fp = fopen(file_name, "wb");
  return_if_fopen_failed(fp,  file_name, "wb");
  int nw = fwrite(Y, sizeof(int), N, fp);
  if ( nw != N ) { go_BYE(-1); } 
  fclose_if_non_null(fp); 
  //======================================
  int64_t old_file_size = get_file_size(file_name); 
  if ( old_file_size != N * sizeof(int) ) { go_BYE(-1); }

  status = stretch(file_name, 2*old_file_size); cBYE(status);
  int64_t new_file_size = get_file_size(file_name); 
  if ( new_file_size != 2 * N * sizeof(int) ) { go_BYE(-1); }

  printf("SUCCESS\n");
BYE:
  // unlink(file_name); 
  // fclose_if_non_null(fp); 
  return status;
}
