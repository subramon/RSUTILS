// gcc thread_cpu_time.c -lm -lpthread
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "q_macros.h"
#include "proc_info.h"
#include "cat_to_buf.h"


/*
 * cat /proc/223249/task/223249/stat
223249 (Rserve) S 1 223249 223249 0 -1 4194368 45 141 0 0 0 1 0 0 20 0 1 0 126577256 66637824 9590 18446744073709551615 94194415439872 94194415526125 140729478943824 0 0 0 0 0 23043 1 0 0 17 2 0 0 0 0 0 94194415560560 94194415579904 94194653057024 140729478946633 140729478946803 140729478946803 140729478950876 0
*/

// Parse /proc/tid/task/tid/stat to find the execution time of the thread
// File format is the same as that of /proc/[pid]/stat
// https://man7.org/linux/man-pages/man5/proc.5.html
char *
proc_info_as_json(
    proc_info_t *ptr_info
    )
{
  int status = 0;
  char tmp[64];
  char *buf = NULL; uint32_t bufsz = 1024; uint32_t buflen = 0;
  buf = malloc(bufsz); bzero(buf, bufsz);
  status = cat_to_buf(&buf, &bufsz, &buflen, "{", 1); cBYE(status);
  sprintf(tmp, " \"utime\" : %lu ", ptr_info->utime); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"stime\" : %lu ", ptr_info->stime); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"cutime\" : %ld ", ptr_info->cutime); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"cstime\"  : %ld ", ptr_info->cstime); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"starttime\" : %llu ", ptr_info->starttime); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"vsize\" : %lu ", ptr_info->vsize); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

  sprintf(tmp, ", \"rss\" : %ld ", ptr_info->rss); 
  status = cat_to_buf(&buf, &bufsz, &buflen, tmp, 0);  cBYE(status);

BYE:
  if ( status != 0 ) { return NULL; } else { return buf; }
}

int 
get_proc_info_for_tid(
    pid_t tid,
    proc_info_t *ptr_info
    ) 
{
#define BUFLEN 63
  int status = 0;
  FILE *fp = NULL;
  char fname[63+1]; // hard coded 
  snprintf(fname, BUFLEN, "/proc/%d/task/%d/stat", tid, tid);
  fp = fopen(fname, "r");
  return_if_fopen_failed(fp, fname, "r");

  // utime = user CPU clock ticks
  // stime = system CPU clock ticks
  int nr = fscanf(fp, 
      "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s "
      "%lu %lu %ld %ld %*s %*s %*s %*s %llu %lu %ld ",
        &(ptr_info->utime),
        &(ptr_info->stime),
        &(ptr_info->cutime),
        &(ptr_info->cstime),
        &(ptr_info->starttime),
        &(ptr_info->vsize),
        &(ptr_info->rss));
  if ( nr != 7 ) { go_BYE(-1); }
BYE:
  fclose_if_non_null(fp);
  return status;
}
/*
(14) utime  %lu
Amount of time that this process has been scheduled
in user mode, measured in clock ticks (divide by
sysconf(_SC_CLK_TCK)).  This includes guest time,
guest_time (time spent running a virtual CPU, see
below), so that applications that are not aware of
the guest time field do not lose that time from
their calculations.

(15) stime  %lu
Amount of time that this process has been scheduled
in kernel mode, measured in clock ticks (divide by
sysconf(_SC_CLK_TCK)).


(16) cutime  %ld
Amount of time that this process's waited-for chil‐
dren have been scheduled in user mode, measured in
clock ticks (divide by sysconf(_SC_CLK_TCK)).  (See
also times(2).)  This includes guest time,
cguest_time (time spent running a virtual CPU, see
below).

(17) cstime  %ld
Amount of time that this process's waited-for chil‐
dren have been scheduled in kernel mode, measured in
clock ticks (divide by sysconf(_SC_CLK_TCK)).

(22) starttime  %llu
The time the process started after system boot.  In
kernels before Linux 2.6, this value was expressed
in jiffies.  Since Linux 2.6, the value is expressed
in clock ticks (divide by sysconf(_SC_CLK_TCK)).

The format for this field was %lu before Linux 2.6.

(23) vsize  %lu
Virtual memory size in bytes.

(24) rss  %ld
Resident Set Size: number of pages the process has
in real memory.  This is just the pages which count
toward text, data, or stack space.  This does not
include pages which have not been demand-loaded in,
or which are swapped out.



*/

