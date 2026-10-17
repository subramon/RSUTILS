#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
int 
pg_get_rslt_set(
    const char * const query,
    const char * const host,
    const char * const db,
    const char * const user,
    const char * const pass,
    char **ptr_json_result
    );
