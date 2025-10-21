#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "q_macros.h"
#include "free_2d_array.h"
#include "csp_info.h"
#include "csp_aux.h"

int
csp_free(
    csp_info_t * ptr_csp_info
    )
{
  int status = 0;

  if ( ptr_csp_info == NULL ) { go_BYE(-1); }
  for ( int i = 0; i < ptr_csp_info->n; i++ ) { 
    switch ( ptr_csp_info->csp_frag[i].csp_src ) { 
      case csp_static : 
        free_if_non_null(ptr_csp_info->csp_frag[i].frag.static_str.str);
        break;
      case csp_Lua_func :
        free_if_non_null(ptr_csp_info->csp_frag[i].frag.Lua_str.func);
        free_if_non_null(ptr_csp_info->csp_frag[i].frag.Lua_str.args);
        free_if_non_null(ptr_csp_info->csp_frag[i].frag.Lua_str.dummy);
        break;
      case csp_C_func : 
        // TODO P2 
        go_BYE(-1);
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  free_2d_array(&(ptr_csp_info->frag_label), ptr_csp_info->n);
  free_if_non_null(ptr_csp_info->csp_frag);
BYE:
  return status;
}
