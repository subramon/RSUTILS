#include <stdio.h>
#include <string.h>
#include <time.h>
#include "q_macros.h"
#include "qtypes.h"
#include "I4_to_TM1.h"

int 
I4_to_TM1(
    int32_t secs_since_epoch,
    bool get_all,
    tm_t *out
    )
{
  int status = 0;
  struct tm *time_struct = NULL;
  time_t seconds = (time_t)secs_since_epoch;

  // Convert seconds to struct tm (local time)
  time_struct = localtime(&seconds);
  if (time_struct == NULL) {
    fprintf(stderr, "Error converting time.\n");
    go_BYE(-1); 
  }

  memset(out, 0, sizeof(tm_t));
  out->tm_year = time_struct->tm_year;
  out->tm_mon  = time_struct->tm_mon;
  out->tm_mday  = time_struct->tm_mday;
  if ( get_all ) { 
    out->tm_hour  = time_struct->tm_hour;
  }
  out->tm_wday  = time_struct->tm_wday;
  out->tm_yday  = time_struct->tm_yday;
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
  int status = 0;
  tm_t out; memset(&out, 0, sizeof(tm_t));
  int32_t seconds = 1741130018;
  status = I4_to_TM1(seconds, false, &out);

  printf("year = %d \n", 1900 + out.tm_year );
  printf("mon = %d \n", out.tm_mon  );
  printf("mday = %d \n", out.tm_mday  );
  printf("hour = %d \n", out.tm_hour );
  printf("wday = %d \n", out.tm_wday);
  printf("yday = %d \n", out.tm_yday);
BYE:
  return status;
}
#endif
