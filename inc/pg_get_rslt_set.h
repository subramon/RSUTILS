#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
int 
pg_get_rslt_set(
    PGconn *in_conn,
    const char * const query,
    const char * const host,
    const char * const db,
    const char * const user,
    const char * const pass,
    char **ptr_json_result
    );
extern int 
pg_get_conn(
    const char * const host,
    const char * const db,
    const char * const user,
    const char * const pass,
    PGconn **ptr_conn
    );
extern int
pg_exec_query_no_rslt(
    PGconn *conn, 
    const char *query
    ) ;
