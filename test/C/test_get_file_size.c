#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "q_incs.h"
#include "get_file_size.h"

int
main(
    void
    )
{
  int status = 0;
  char *X = NULL;
  const char *file_name = "_xxx.bin";
  int width = (int)sizeof(int);
  for ( int i = 1; i < 100; i++ ) { 
    // create some dummy data 
    X = malloc(i*sizeof(int));
    return_if_malloc_failed(X);
    for ( int j = 0; j < i; j++ ) { 
      X[j] = i+1;
    }
    // write dummy data to a file 
    FILE *fp = NULL;
    fp = fopen(file_name, "wb");
    return_if_fopen_failed(fp,  file_name, "wb");
    int nw = fwrite(X, sizeof(int), i, fp);
    if ( nw != i ) { go_BYE(-1); } 
    fclose_if_non_null(fp);
    // check file size 
    int64_t file_size = get_file_size(file_name);
    if ( file_size != (width * i) ) { go_BYE(-1); }
    free(X); X = NULL;
    unlink(file_name); 
  }
  printf("SUCCESS\n");
BYE:
  unlink(file_name);
  free_if_non_null(X);
  return status;
}
