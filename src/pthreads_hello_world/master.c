#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker.h"


int 
main(
    int argc,
    char ** argv
    ) 
{
  int status = 0;
  pthread_t thread_id;
  void* thread_result;

  // 1. Create the worker thread
  if (pthread_create(&thread_id, NULL, worker, NULL) != 0) {
    perror("Failed to create thread");
    return EXIT_FAILURE;
  }

  // 2. Let the worker thread run for 3 seconds
  sleep(3);

  // 3. Send a cancellation request to the worker thread
  printf("Main thread: Sending cancellation request to worker thread...\n");
  if (pthread_cancel(thread_id) != 0) {
    perror("Failed to cancel thread");
    return EXIT_FAILURE;
  }

  // 4. Wait for the worker thread to terminate and check its exit status
  if (pthread_join(thread_id, &thread_result) != 0) {
    perror("Failed to join thread");
    return EXIT_FAILURE;
  }
  /*
  if ( thread_result != NULL ) { 
    worker_results_t *W = (worker_results_t *)thread_result;
    printf("i4 = %d \n", W->i4);
    printf("f8 = %lf \n", W->f8);
    printf("str = %s \n", W->str);
    free(W->str);
    free(W);
  }
  */

  // 5. Verify the thread was actually cancelled
  if (thread_result == PTHREAD_CANCELED) {
    printf("Main thread: Success! Worker thread was successfully cancelled.\n");
  } else {
    printf("Main thread: Worker thread exited normally (unexpected).\n");
  }

BYE:
  return status;
}
