#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"
#include "csp_info.h"

#include "cat_to_buf.h"
#include "csp_synthesize.h"

int
csp_synthesize(
    const csp_info_t * const ptr_csp_info,
    char **ptr_out_str,
    uint32_t *ptr_out_len
    )
{
  int status = 0;
  char *out_str = NULL; uint32_t out_len = 0;
  uint32_t out_sz = 1024;

  *ptr_out_str = NULL;
  *ptr_out_len = 0;
  out_str = malloc(out_sz); memset(out_str, 0, out_sz);

  for ( uint32_t i = 0; i < ptr_csp_info->n; i++ ) {
    switch ( ptr_csp_info->csp_frag[i].csp_src ) { 
      case csp_static: 
        status = cat_to_buf(&out_str, &out_sz, &out_len, 
          ptr_csp_info->csp_frag[i].frag.static_str.str, 
          ptr_csp_info->csp_frag[i].frag.static_str.len);
        cBYE(status);
        break;
      case csp_Lua_func : 
        status = cat_to_buf(&out_str, &out_sz, &out_len, 
          ptr_csp_info->csp_frag[i].frag.Lua_str.dummy, 0);
        cBYE(status);
        break;
      case csp_C_func : 
        // TODO 
        go_BYE(-1); 
        break;
      default : 
        go_BYE(-1); 
        break;
    }
  }
  *ptr_out_str = out_str;
  *ptr_out_len = out_len;
BYE:
  return status;
}
