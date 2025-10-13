#include <stdio.h>
#include <lua.h>
#include <stdlib.h>
#include <stdint.h>
#include "q_macros.h"
#include "csp_info.h"

#include "rs_mmap.h"
#include "cat_to_buf.h"
#include "csp_gen.h"
int
csp_gen(
    lua_State *L,
    const csp_info_t *ptr_csp_info,
    char **ptr_str
    )
{
  int status = 0;
  char *str = NULL; uint32_t len = 1024, sz = 0;

  str = malloc(len); memset(str, 0, len);

  char *X = NULL; size_t nX = 0;
  size_t start, stop;

  for ( uint32_t i = 0; i <  ptr_csp_info->n; i++ ) { 
    switch ( ptr_csp_info->csp_frag[i].csp_src ) {
      case csp_static : 
        status = cat_to_buf(&str, &sz, &len, 
          ptr_csp_info->csp_frag[i].frag.static_str.str,
          ptr_csp_info->csp_frag[i].frag.static_str.len);
        cBYE(status);
        break;
      case csp_Lua_func : 
        break;
      case csp_C_func : 
        break;
      default :
        go_BYE(-1);
        break;
    }
  }

  



  *ptr_str = str;
BYE:
  return status;
}

