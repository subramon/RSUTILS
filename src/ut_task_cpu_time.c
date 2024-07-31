#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "task_cpu_time.h"

void *
do_little_work(
    void *arg
    ) 
{
  int i,j;
  float foo;
  pid_t tid = get_task_id();

  // Do more work
  for (j=0; j < 5000; j++){
    for (i=0; i < 5000; i++){
      foo += sin(i) / exp(-1 * i);
    }
  }
  sleep(5);
  int cpu = get_task_cpu_time(tid);
  printf("Little work: TID=%d CPU=%.4fs\n", tid, 
      (double)cpu *1.0/sysconf(_SC_CLK_TCK));
  pthread_exit(NULL);
}

void *
do_more_work(
    void *arg
    ) 
{
  int i,j;
  double foo;
  pid_t tid = get_task_id();

  // Do more work
  for (j=0; j < 30000; j++){
    for (i=0; i < 5000; i++){
      foo += sin(i) / exp(-1 * i);
    }
  }

  int cpu = get_task_cpu_time(tid);
  printf("More work: TID=%d CPU=%.4fs\n", tid, cpu*1.0/sysconf(_SC_CLK_TCK));

  pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    pthread_t p1;
    pthread_t p2;

    pthread_create(&p1, NULL, &do_more_work, NULL);
    pthread_create(&p2, NULL, &do_little_work, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
}
