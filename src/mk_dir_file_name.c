#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"
#include "mk_dir_file_name.h"

//START_FUNC_DECL
int
mk_dir_file_name(
    const char * const d,
    const char * const f,
    char **ptr_fname
    )
//STOP_FUNC_DECL
{
  int status = 0;
  *ptr_fname = NULL;
  char *fname = NULL; 

  if ( ( f == NULL ) || ( *f == '\0' ) ) { go_BYE(-1); }
  int len = strlen(f) + 8; // +8 for kosuru
  if ( ( d != NULL ) && ( *d != '\0' ) ) {
    len += strlen(d);
  }
  fname = malloc(len); memset(fname, 0, len);
  if ( ( d != NULL ) && ( *d != '\0' ) ) {
    sprintf(fname, "%s/%s", d, f);
  }
  else {
    strcpy(fname, f);
  }
  *ptr_fname = fname;
BYE:
  return status;
}
