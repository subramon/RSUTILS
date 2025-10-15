#include <lua.h>
#include <stdint.h>
#include <stdbool.h>
#include "csp_info.h"
extern int
csp_synthesize(
    lua_State *L,
    const csp_info_t * const ptr_csp_info,
    bool use_dummy,
    bool use_cache,
    char **ptr_out_str,
    uint32_t *ptr_out_len
    );
