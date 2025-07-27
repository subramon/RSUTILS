#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "q_macros.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int *X = NULL;
  char fname[16];
  FILE *fp = NULL;

  int id = atoi(argv[1]); 
  // printf(">>>>>> Child mypid = %d arg = %s\n", getpid(), argv[1]); 
  sprintf(fname, "_%d.txt", getpid());
  fp = fopen(fname, "w");
  fprintf(fp, "hello world\n");
  fclose(fp); 
  sleep(1); 
  X = malloc(1024);
  for ( int i = 0; i < 1024/sizeof(int); i++ ) { X[i] = i; }
  int sum = 0;
  for ( int i = 0; i < 1024/sizeof(int); i++ ) { sum += X[i]; }
  if ( ( id % 2 ) == 0 ) { 
    printf("Deliberate failure of child %d, id = %d \n", getpid(), id);
    for ( int i = 0; i < 1024; i++ ) { sum += X[i]; }
    free(X); 
    free(X); 
  }
  free(X);
  printf("Child %d succeeded %d, sum = %d\n", getpid(), id, sum);
BYE:
  return status;
}
