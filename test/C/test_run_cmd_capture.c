#include <stdio.h>
#include <stdlib.h>
#include "run_cmd_capture.h"

int main(void)
{
  int   status;
  char *out = NULL, *err = NULL;

  if (run_cmd_capture("ls /nonexistent ; echo hello", &status, &out, &err) != 0) {
    perror("run_cmd_capture");
    return 1;
  }

  printf("status = %d\n", status);
  printf("--- stdout ---\n%s", out);
  printf("--- stderr ---\n%s", err);

  free(out);
  free(err);
  return 0;
}
