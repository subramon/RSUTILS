#include "q_incs.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "multiple.h"
#include "rdtsc.h"
#include "mk_temp_file_name.h"

// Created because I *suspected* some strange results with mkstemp

char *
mk_temp_file_name(
    const char * const prefix,
    const char * const suffix
    )
{
  int status = 0;
  size_t len = 60;
  if ( prefix != NULL ) { len += strlen(prefix); }
  if ( suffix != NULL ) { len += strlen(suffix); }
  len = multiple_n(len, 8);
  char *x = malloc(len); memset(x, 0, len);

  if ( prefix == NULL ) { 
    if ( suffix  == NULL ) { 
      sprintf(x, "_%" PRIu64 "_%lu", RDTSC(), syscall(SYS_gettid));
    }
    else {
     sprintf(x, "_%" PRIu64 "_%lu_%s", 
        RDTSC(), syscall(SYS_gettid), suffix);
    }
  }
  else {
    if ( suffix == NULL ) { 
      sprintf(x, "%s_%" PRIu64 "_%lu", prefix, 
        RDTSC(), syscall(SYS_gettid));
    }
    else {
      sprintf(x, "%s_%" PRIu64 "_%lu_%s", prefix, 
        RDTSC(), syscall(SYS_gettid), suffix);
    }
  }
BYE:
  if ( status != 0 ) { return NULL; } else { return x; } 
}

#undef TEST
#ifdef TEST
int
main(
    void
    )
{
  char *X = NULL; 
  X = mk_temp_file_name("/tmp/_x", ".json");
  printf("%s\n", X);
BYE:
  free_if_non_null(X);
}

#endif
