#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"
#include "touch.h"

// touch like in Unix
int
touch(
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;
  if ( ( file_name == NULL ) || ( *file_name == '\0' ) ) { go_BYE(-1); }
  fp = fopen(file_name, "w");
  return_if_fopen_failed(fp,  file_name, "w");
  fclose_if_non_null(fp);
BYE:
  return status;
}
