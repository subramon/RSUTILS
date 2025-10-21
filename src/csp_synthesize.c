#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"
#include "csp_info.h"

#include "cat_to_buf.h"
#include "csp_synthesize.h"
#include "bridge_csp.h"

int
csp_synthesize(
    lua_State *L,
    const csp_info_t * const ptr_csp_info,
    bool use_dummy,
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

  for ( int i = 0; i < ptr_csp_info->n; i++ ) {
    switch ( ptr_csp_info->csp_frag[i].csp_src ) { 
      case csp_static: 
        status = cat_to_buf(&out_str, &out_sz, &out_len, 
            ptr_csp_info->csp_frag[i].frag.static_str.str, 
            (uint32_t)ptr_csp_info->csp_frag[i].frag.static_str.len);
        cBYE(status);
        break;
      case csp_Lua_func : 
        if ( ( use_dummy ) && 
            ( ptr_csp_info->csp_frag[i].frag.Lua_str.dummy != NULL ) ) { 
          status = cat_to_buf(&out_str, &out_sz, &out_len, 
              ptr_csp_info->csp_frag[i].frag.Lua_str.dummy, 0);
          cBYE(status);
        }
        else {
          char *str = NULL;
          const char * func = ptr_csp_info->csp_frag[i].frag.Lua_str.func;
          if ( strcmp(func, "<func being tested>") == 0 ) { // for testing 
            printf("Faking generator for %s \n", func); 
            status = cat_to_buf(&out_str, &out_sz, &out_len, 
                ptr_csp_info->csp_frag[i].frag.Lua_str.dummy, 0);
            cBYE(status);
          }
          else { // This is the normal case 
            status = bridge_csp(L, 
                ptr_csp_info->csp_frag[i].frag.Lua_str.func, 
                ptr_csp_info->csp_frag[i].frag.Lua_str.args, &str); 
            cBYE(status);
            status = cat_to_buf(&out_str, &out_sz, &out_len, str, 0);
            cBYE(status);
            free_if_non_null(str);
          }
        }
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
