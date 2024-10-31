#include "q_incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "isfile.h"
#include "load_configs.h"

int
load_configs(
    lua_State *L,
    const char * const cfg_file
    )
{
  int status = 0;
  char *cmd = NULL;

  if ( cfg_file == NULL ) { go_BYE(-1); }
  if ( !isfile(cfg_file) ) { 
    fprintf(stderr, "File [%s] \not found \n", cfg_file); go_BYE(-1); 
  }
  if ( L == NULL ) { go_BYE(-1); }

  uint32_t len = (uint32_t)strlen(cfg_file) + 64; 
  cmd = malloc(len); 
  return_if_malloc_failed(cmd);
  memset(cmd, 0, len); 
  // Load the configs into glC. IMPORTANT: This is a global 
  sprintf(cmd, "x = loadfile('%s')\n", cfg_file);
  status = luaL_dostring(L, cmd);
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
#ifdef DEBUG
  status = luaL_dostring(L, "assert(type(x) == 'function')");
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
#endif
  status = luaL_dostring(L, "glC = x()\n");
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
#ifdef DEBUG
  status = luaL_dostring(L, "assert(type(glC) == 'table')");
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
#endif
  //---------------------------------------------
  status = luaL_dostring(L, "if ( glC.is_strict ) then require 'strict' end");
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
  
BYE:
  free_if_non_null(cmd);
  return status;
}
