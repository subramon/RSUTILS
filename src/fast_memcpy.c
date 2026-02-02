// TODO: Integrate this into code 
 // gcc x.c /opt/intel/oneapi/compiler/2022.0.2/linux/compiler/lib/intel64_lin/libirc.a -o a.out
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "rdtsc.h"

int
main(
    int argc,
    char **argv
    )
{
  int N = 1048576;
  int *X = NULL, *Y = NULL;
  X = malloc(N * sizeof(int));
  Y = malloc(N * sizeof(int));

  uint64_t t1 = rdtsc();
  for ( int i = 0; i < 100; i++ ) { 
    memcpy(X, Y, N * sizeof(int));
  }
  printf("memcpy = %lu \n", rdtsc() - t1); 

  t1 = rdtsc();
  for ( int i = 0; i < 20; i++ ) { 
    _intel_fast_memcpy(X, Y, N * sizeof(int));
  }
  printf("_intel_fast_memcpy = %lu \n", rdtsc() - t1); 
}

