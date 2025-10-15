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
        free_if_non_null(ptr_csp_info->csp_frag[i].frag.Lua_str.cache);
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
int
csp_uncache(
    csp_info_t *csp_info, // [n_csp]
    char **csp_labels, // [n_csp]
    uint32_t n_csp,
    const char * const csp_label, 
    const char * const frag_label
    )
{
  int status = 0;
  if ( csp_info == NULL ) { go_BYE(-1); }
  if ( n_csp == 0 ) { go_BYE(-1); }
  if ( csp_label  == NULL ) { go_BYE(-1); }
  if ( frag_label == NULL ) { go_BYE(-1); }

  int cidx = -1;
  for ( uint32_t i = 0; i < n_csp; i++ ) { 
    if ( strcmp(csp_label, csp_labels[i]) == 0 ) {
      cidx = i; break;
    }
  }
  if ( cidx < 0 ) { go_BYE(-1); }

  int fidx = -1;
  for ( int i = 0; i < csp_info[i].n; i++ ) { 
    if ( strcmp(frag_label, csp_info[cidx].frag_label[i]) == 0 ) {
      fidx = i; break;
    }
  }
  if ( fidx < 0 ) { go_BYE(-1); }

  free_if_non_null(csp_info[cidx].csp_frag[fidx].frag.Lua_str.cache);
  csp_info[cidx].csp_frag[fidx].frag.Lua_str.len = 0; 

BYE:
  return status;
}
