#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker.h"

// Shared atomic flag
_Atomic bool g_is_finished;

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
  g_is_finished = false;
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
  // 5. Verify the thread was actually cancelled
  if (thread_result == PTHREAD_CANCELED) {
    printf("Main thread: Success! Worker thread was successfully cancelled.\n");
  } else {
    printf("Main thread: Worker thread exited normally (unexpected).\n");
  }
  printf("Case 1 completed\n\n\n");
  // CASE 2: Worker thread is detached from main thread 
  printf("Case 2 started: detached worker \n");
  g_is_finished = false;
  pthread_attr_t attr;
  // 1. Initialize the thread attributes structure
  if (pthread_attr_init(&attr) != 0) {
    perror("Failed to initialize thread attributes");
    return EXIT_FAILURE;
  }
  // 2. Set the detach state attribute to DETACHED
  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
    perror("Failed to set detach state");
    pthread_attr_destroy(&attr); // Clean up attribute before exit
    return EXIT_FAILURE;
  }
  // 3. Create the thread passing the configured attributes
  pthread_t p2; 
  printf("Main: Creating detached background worker thread...\n");
  if (pthread_create(&p2, &attr, worker, NULL) != 0) {
      perror("Failed to create thread");
      pthread_attr_destroy(&attr);
      return EXIT_FAILURE;
  }
  pthread_attr_destroy(&attr);
  // 5. Keep main alive long enough to watch the detached thread run.
  // If main exits immediately, the entire process (and all threads) will die.
  printf("Main: Thread spawned. Continuing main operations...\n");
  sleep(3);
  // Now cancel the worker 
  printf("Main thread: Sending cancellation request to worker thread...\n");
  if (pthread_cancel(p2) != 0) {
    perror("Failed to cancel thread");
    return EXIT_FAILURE;
  }
    while( g_is_finished == false) { 
      printf("Master waiting for detached worker to cleanup\n");
      sleep(1);
    }
  // Notice no pthread_join 

BYE:
  return status;
}
