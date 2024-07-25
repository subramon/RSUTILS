#include "q_incs.h"
#include "rand_file_name.h"

int
main()
{
  int status = 0;
#define N 64
  char X[N];
  for ( int i = 0; i < 10; i++ ) { 
    status = rand_file_name(X, N); cBYE(status);
    fprintf(stderr, "X = %s \n", X);
  }
  printf("SUCCESS\n");
BYE:
  return status;
}
