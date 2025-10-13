#include <lua.h>
#include "csp_info.h"
extern int
csp_gen(
    lua_State *L,
    const csp_info_t *ptr_csp_info,
    char **ptr_str
    );
