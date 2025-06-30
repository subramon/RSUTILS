// gcc thread_cpu_time.c -lm -lpthread
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "q_macros.h"
#include "task_cpu_time.h"

pid_t 
get_task_id(
    void
    ) 
{
  pid_t tid = syscall(SYS_gettid);
  return tid;
}

// Parse /proc/self/task/tid/stat to find the execution time of the thread
// File format is the same as that of /proc/[pid]/stat
// https://man7.org/linux/man-pages/man5/proc.5.html
int 
get_task_cpu_time(
    pid_t tid
    ) 
{
  char fname[200];
  snprintf(fname, sizeof(fname), "/proc/self/task/%d/stat", (int) get_task_id());
  FILE *fp = fopen(fname, "r");
  if ( !fp ) return -1;

  // ucpu = user CPU clock ticks
  // scpu = system CPU clock ticks
  // tot_cpu = total CPU clock ticks 
  int ucpu = 0, scpu=0, tot_cpu = 0;
  if ( fscanf(fp, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %d %d",
        &ucpu, &scpu) == 2 ){
    printf("ucpu=%d, scpu=%d\n", ucpu, scpu);
    tot_cpu = ucpu + scpu; 
  }
  else
    printf("Unable to read stat file\n");

  fclose(fp);
  return tot_cpu;
}

// Parse /proc/self/task/tid/stat to find the execution time of the thread
// File format is the same as that of /proc/[pid]/stat
// https://man7.org/linux/man-pages/man5/proc.5.html
int 
get_utime_stime_for_tid(
    pid_t tid,
    uint64_t *ptr_utime,
    uint64_t *ptr_stime
    ) 
{
#define BUFLEN 63
  int status = 0;
  FILE *fp = NULL;
  char fname[63+1]; // hard coded 
  snprintf(fname, BUFLEN, "/proc/self/task/%d/stat", tid);
  fp = fopen(fname, "r");
  return_if_fopen_failed(fp, fname, "r");

  // utime = user CPU clock ticks
  // stime = system CPU clock ticks
  int nr = fscanf(fp, 
      "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %" PRIu64 " %" PRIu64 "\n", 
        ptr_utime, ptr_stime);
  if ( nr != 2 ) { go_BYE(-1); }
BYE:
  fclose_if_non_null(fp);
  return status;
}
/*
     * Explanation of utime, stime
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
*/

