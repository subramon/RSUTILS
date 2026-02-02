#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "q_macros.h"
#include "get_time_usec.h"
#include "rdtsc.h"
#ifndef ARM
#include "x86intrin.h"
#endif

//START_FUNC_DECL
uint64_t
rdtsc(
    void
    )
//STOP_FUNC_DECL
{
#ifndef ARM
  return __rdtsc();
#else
  return get_time_usec();
#endif
}
