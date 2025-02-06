#include <lua.h>
#include "q_incs.h"
#include "web_struct.h"
#include "free_web_info.h"
#include "free_2d_array.h"

int 
free_web_info(
    web_info_t *ptr_W
    )
{
  int status = 0;
  if ( ptr_W == NULL ) { go_BYE(-1); }
  free_if_non_null(ptr_W->docroot);
  free_if_non_null(ptr_W->login_page);
  free_if_non_null(ptr_W->login_endp);
  free_if_non_null(ptr_W->init_lua_state);
  free_2d_array(&(ptr_W->users), ptr_W->n_users);
  // Internals of sess_state should be freed by user
  if ( ptr_W->sess_state != NULL ) { 
    for ( uint32_t i = 0; i < ptr_W->n_users; i++ ) { 
      lua_State *L = ptr_W->sess_state[i].L;
      if ( L != NULL ) { lua_close(L); }
      // NOTE: Anything else in sess_state is application specific
      // and should be freed by application
    }
  }
  free_if_non_null(ptr_W->sess_state);
  free_if_non_null(ptr_W->in_use);
  memset(ptr_W, 0, sizeof(web_info_t));
BYE:
  return status;
}
