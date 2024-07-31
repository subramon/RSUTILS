// gcc thread_cpu_time.c -lm -lpthread
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

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
