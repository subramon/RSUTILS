#include "q_incs.h"
#include "_mix_UI8.h"
#include "_rand_file_name.h"

int
main()
{
  char X[32];
  for ( int i = 0; i < 10; i++ ) { 
    rand_file_name(X, 32);
    fprintf(stderr, "X = %s \n", X);
  }
}
