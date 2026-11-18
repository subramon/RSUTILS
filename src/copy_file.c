#include <libgen.h>
#include "q_incs.h"
#include "q_macros.h"
#include "isfile.h"
#include "isdir.h"
#include "rs_mmap.h"
#include "copy_file.h"
int
copy_file(
    const char * const src,
    const char * const dst
    )
{
  int status = 0; 
  FILE *fp = NULL;
  char *X = NULL; size_t nX = 0;
  char *x_dst = NULL; 

  if ( !isfile(src) ) { go_BYE(-1); }
  if ( isdir(dst) ) {
    const char *dir = dst; // we have been given directory not file
    const char *basename_src = strrchr(src, '/');
    basename_src = ( basename_src != NULL ) ? basename_src + 1 : src;
    size_t len = strlen(dir) + strlen(basename_src) + 8;
    x_dst = malloc(len * sizeof(*x_dst));
    return_if_malloc_failed(x_dst);
    int nw = snprintf(x_dst, len, "%s/%s", dir, basename_src);
    if ( ( nw < 0 ) || ( (size_t)nw >= len ) ) { go_BYE(-1); }
  }
  else {
    x_dst = strdup(dst);
  }
  if ( strcmp(x_dst, src) == 0 ) { go_BYE(-1); }
  status = rs_mmap(src, &X, &nX, 0); cBYE(status);
  fp = fopen(x_dst, "w");
  // printf("%s, %s, %s \n", src, dst, x_dst);
  return_if_fopen_failed(fp, x_dst, "w");
  fwrite(X, nX, 1, fp);
BYE:
  fclose_if_non_null(fp);
  free_if_non_null(x_dst);
  mcr_rs_munmap(X, nX);
return status;
}
