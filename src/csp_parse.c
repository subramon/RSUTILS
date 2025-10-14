#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "q_macros.h"
#include "csp_info.h"

#include "rs_mmap.h"
#include "file_as_str.h"
#include "csp_parse.h"
int
csp_parse(
    const char * const csp_file,
    csp_info_t *ptr_csp_info

    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  char *str = NULL, *bak_str = NULL; size_t len = 0;
  size_t start, stop;

  status = file_as_str(csp_file, &str, &len); cBYE(status);
  bak_str = str;
  int num_frags = 0; 
  int frag_idx = 0;
  for ( int pass = 1; pass <= 2; pass++ ) { 
    char *cptr = strstr(str, "<?Lua START"); 
    if ( cptr == NULL ) { 
      /* everything goes into static str */
      if ( pass == 1 ) { 
        num_frags++; 
      }
      else {
        ptr_csp_info->csp_frag[frag_idx].csp_src = csp_static;
        ptr_csp_info->csp_frag[frag_idx].frag.static_str.str=strdup(cptr); 
        ptr_csp_info->csp_frag[frag_idx].frag.static_str.len=strlen(cptr); 
      }
      break; // all done 
    }
  }
  



BYE:
  mcr_rs_munmap(X, nX);
  free_if_non_null(bak_str);
  return status;
}

