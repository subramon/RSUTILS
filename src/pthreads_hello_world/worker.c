#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker.h"


void cleaner(
    void* arg
    ) 
{
  char* to_free = (char*)arg;

  printf("Cleanup started\n");
  // 1. Free memory safely
  if (to_free != NULL) {
    free(to_free);
    printf("Cleanup: Successfully freed memory buffer.\n");
  }

  // 2. Unlock mutex to prevent deadlocking other threads
  // if any global sharing is likely to have happened
  printf("Cleanup done\n");
}

void* 
worker(
    void* arg
    ) 
{
  char *to_free = NULL; to_free = malloc(1024);
  
  /*
  worker_results_t *to_return = NULL; 
  to_return = malloc(1 * sizeof(worker_results_t));
  to_return->i4 = 10; 
  to_return->f8 = 20;
  to_return->str = strdup("Hello world, worker responding\n");
  */
  // Set cancellation to enabled (this is the default, but good practice to explicitly state)
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  // Set cancellation type to deferred (cancels only at specific cancellation points)
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  pthread_cleanup_push(cleaner, to_free);


  printf("Worker thread: Started and entering infinite loop...\n");

  int counter = 0;
  while (1) {
    printf("Worker thread: Working... (Iteration %d)\n", ++counter);
    struct timespec  tmspec = { .tv_sec = 1, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    pthread_testcancel();


    // sleep() is a POSIX cancellation point where the thread checks for a cancel request
    // So also XXX
    
  }
  pthread_cleanup_pop(0);
  return NULL; 
}
