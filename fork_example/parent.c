#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "q_macros.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  pid_t pid = -1;

  for ( int i = 0; i < 10; i++ ) {
    pid = fork();
    if ( pid == -1 ) { 
      perror("fork failed"); go_BYE(-1); 
    }
    pid_t mypid = getpid();
    pid_t ppid  = getppid();
    // printf("mypid = %d, ppid = %d \n", mypid, ppid); 
    if ( pid == 0 ) {
      char dummy[16];
      sprintf(dummy, "%d", i+1);
      const char *args[] = {"child", dummy, NULL};
      // Execute a new program (e.g., "ls -l")
      // execvp searches for the executable in the PATH environment variable
      if (execvp(args[0], (char **)args) == -1) {
        // If execvp returns, it means an error occurred
        perror("execvp failed");
        go_BYE(-1); 
      }
    }
    sleep(1);
    // printf("Spawned child %d \n", i+1);
  }
  sleep(5);
BYE:
  return status;
}
