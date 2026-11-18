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
  size_t len = strlen(f) + 8; // +8 for kosuru
  if ( ( d != NULL ) && ( *d != '\0' ) ) {
    len += strlen(d);
  }
  fname = malloc(len); return_if_malloc_failed(fname);
  memset(fname, 0, len);
  if ( ( d != NULL ) && ( *d != '\0' ) ) {
    int nw = snprintf(fname, len, "%s/%s", d, f);
    if ( ( nw < 0 ) || ( (size_t)nw >= len ) ) { go_BYE(-1); }
  }
  else {
    memcpy(fname, f, strlen(f) + 1);
  }
  *ptr_fname = fname;
BYE:
  return status;
}
