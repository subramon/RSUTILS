// Assumptions 
// 1. C passes address of config struct to Lua
// 2. A Lua function populates the struct 
// 3. Lua function must have necessary cdefs to understand struct
// 4. Lua function returns true if all goes well
#include "q_incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "q_macros.h"
#include "mk_lua_state.h"
#include "isfile.h"
#include "load_configs.h"
#include "bridge_read_configs.h"

int
bridge_read_configs(
    const char * const cfg_file,
    void *C,
    const char * const lua_fn
    )
{
  int status = 0; 
  lua_State *L = NULL;
  int  chk; 
  char lcmd[128];
  // basic checks
  if ( cfg_file == NULL ) { go_BYE(-1); }
  if ( !isfile(cfg_file) ) { go_BYE(-1); }
  if ( lua_fn == NULL ) { go_BYE(-1); }
  // create Lua state
  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  // Load the configs into G
  status = load_configs(L, cfg_file); cBYE(status); 
  // Load function to be used for reading 
  sprintf(lcmd, "rdfn = require '%s'", lua_fn); 
  status = luaL_dostring(L, lcmd); 
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
  // Put lua read_configs function on stack 
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }
  lua_getglobal(L, "rdfn");
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function rdfn undefined\n");
    lua_pop(L, 1);
    go_BYE(-1);
  }
  // put config file and pointer to C struct config on stack 
  lua_pushlightuserdata(L, C);
  chk = lua_gettop(L); if ( chk != 2 ) { go_BYE(-1); }
  // call lua function and check status 
  status = lua_pcall(L, 1, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "fn %s failed: %s\n", lua_fn, lua_tostring(L, -1));
    lua_pop(L, 1);
    go_BYE(-1); 
  }
  // check return value which should be true 
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isboolean(L, 1) ) {
    bool rslt = lua_toboolean(L, -1);
    if ( !rslt ) { go_BYE(-1); }
  }
  // clean up lua stack 
  lua_pop(L, 1);
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }
  // clean up Loa state of stuff created in load_configs()
  status = luaL_dostring(L, "glC = nil; x = nil;\n"); 
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  cBYE(status); 
BYE:
  if ( L != NULL ) { lua_close(L); L = NULL; }
  return status;
}
