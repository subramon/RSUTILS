#include "q_incs.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "multiple.h"
#include "rdtsc.h"
#include "mk_temp_file_name.h"

// Created because I *suspected* some strange results with mkstemp

int
mk_temp_file_name(
    const char * const prefix,
    const char * const suffix,
    char **ptr_name
    )
{
  int status = 0;
  size_t len = 60;
  *ptr_name = NULL;
  if ( prefix != NULL ) { len += strlen(prefix); }
  if ( suffix != NULL ) { len += strlen(suffix); }
  len = multiple_n(len, 8);
  char *x = malloc(len); return_if_malloc_failed(x);
  memset(x, 0, len);
  const uint64_t stamp = rdtsc();
  const unsigned long tid = (unsigned long)syscall(SYS_gettid);
  int nw = 0;

  if ( prefix == NULL ) {
    if ( suffix == NULL ) {
      nw = snprintf(x, len, "_%" PRIu64 "_%lu", stamp, tid);
    }
    else {
      nw = snprintf(x, len, "_%" PRIu64 "_%lu_%s", stamp, tid, suffix);
    }
  }
  else {
    if ( suffix == NULL ) {
      nw = snprintf(x, len, "%s_%" PRIu64 "_%lu", prefix, stamp, tid);
    }
    else {
      nw = snprintf(x, len, "%s_%" PRIu64 "_%lu_%s", prefix, stamp, tid, suffix);
    }
  }
  if ( ( nw < 0 ) || ( (size_t)nw >= len ) ) { free_if_non_null(x); go_BYE(-1); }
  *ptr_name = x; 
BYE:
  return status; 
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
