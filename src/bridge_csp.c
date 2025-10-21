#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <lua.h>
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
  int ntop;
  if ( L == NULL ) { WHEREAMI; return -1; } 
  if ( lua_gettop(L) != 0 ) { go_BYE(-1); }
  //---------------------------------------------------------
  if ( func == NULL ) { go_BYE(-1); }
  lua_getglobal(L, func);
  //---------------------------------------------------------
  if ( args == NULL ) { 
    lua_pushnil(L);
  }
  else {
    lua_pushstring(L, args); 
  }
  //---------------------------------------------------------
  status = lua_pcall(L, 1, 1, 0);
  if ( status != 0 ) { 
    fprintf(stderr, "%s() failed: %s\n", func, lua_tostring(L, -1));
    go_BYE(-1);
  }
  //---------------------------------------------------------
  if ( !lua_isstring(L, 1) ) { 
    go_BYE(-1); }
  *ptr_str = strdup(lua_tostring(L, -1));
  //---------------------------------------------------------
BYE:
  ntop = lua_gettop(L); 
  if ( ntop != 0 ) { lua_pop(L, ntop); }
  return status;
}
