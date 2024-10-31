#include "q_incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "mk_lua_state.h"

#define MAX_LEN_DIR_NAME 127

lua_State *
mk_lua_state(
    void
    )
{
  int status = 0;
  lua_State *L = NULL;

  L = luaL_newstate();
  if ( L == NULL ) { go_BYE(-1); }
  luaL_openlibs(L);
BYE:
  if ( status < 0 ) { return NULL; } else { return L; } 
}
