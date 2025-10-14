#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "q_macros.h"
#include "csp_info.h"

#include "bridge_csp.h"
int
bridge_csp(
    lua_State *L,
    const char * const func,
    const char * const args,
    char **ptr_str // output 
    )
{
  int status = 0;
  if ( L == NULL ) { WHEREAMI; return -1; } 
  if ( lua_gettop(L) != 0 ) { go_BYE(-1); }
  //---------------------------------------------------------
  lua_getglobal(L, func);
  status = lua_pcall(L, 0, 1, 0);
  if ( status != 0 ) { 
    fprintf(stderr, "get_mode() failed: %s\n", lua_tostring(L, -1));
    go_BYE(-1);
  }
  //---------------------------------------------------------
  if ( func == NULL ) { go_BYE(-1); }
  lua_pushstring(L, func); 
  if ( args == NULL ) { 
    lua_pushnil(L);
  }
  else {
  lua_pushstring(L, args); 
  }
  //---------------------------------------------------------
  status = lua_pcall(L, 2, 1, 0);
  if ( status != 0 ) { 
    fprintf(stderr, "get_mode() failed: %s\n", lua_tostring(L, -1));
    go_BYE(-1);
  }
  //---------------------------------------------------------
  if ( !lua_isstring(L, 1) ) { go_BYE(-1); }
  *ptr_str = strdup(lua_tostring(L, -1));
  //---------------------------------------------------------
BYE:
  ntop = lua_gettop(L); 
  if ( ntop != 0 ) { lua_pop(L, ntop); }
  return status;
}
