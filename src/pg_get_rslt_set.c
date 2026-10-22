#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "q_macros.h"
#include "pg_get_rslt_set.h"

// Helper structure for dynamic string buffer
typedef struct {
    char *data;
    size_t len;
    size_t capacity;
} StringBuffer;

static 
void init_buffer(
    StringBuffer *sb, 
    size_t initial_capacity
    ) 
{
  sb->data = malloc(initial_capacity);
  sb->data[0] = '\0';
  sb->len = 0;
  sb->capacity = initial_capacity;
}

static 
void append_buffer(
    StringBuffer *sb, 
    const char *fmt, 
    ...)
{
  va_list args;
  va_start(args, fmt);

  // Determine required length
  va_list args_copy;
  va_copy(args_copy, args);
  int needed = vsnprintf(NULL, 0, fmt, args_copy);
  va_end(args_copy);

  if (needed < 0) {
    va_end(args);
    return;
  }

  if (sb->len + needed + 1 >= sb->capacity) {
    sb->capacity = (sb->len + needed + 1) * 2;
    sb->data = realloc(sb->data, sb->capacity);
  }

  vsnprintf(sb->data + sb->len, sb->capacity - sb->len, fmt, args);
  sb->len += needed;
  va_end(args);
}

// Function to escape special JSON characters in values
static 
void json_escape_append(
    StringBuffer *sb, 
    const char *val
    ) 
{
    append_buffer(sb, "\"");
    if (val) {
        for (int i = 0; val[i] != '\0'; i++) {
            char c = val[i];
            switch (c) {
                case '\"': append_buffer(sb, "\\\""); break;
                case '\\': append_buffer(sb, "\\\\"); break;
                case '\b': append_buffer(sb, "\\b"); break;
                case '\f': append_buffer(sb, "\\f"); break;
                case '\n': append_buffer(sb, "\\n"); break;
                case '\r': append_buffer(sb, "\\r"); break;
                case '\t': append_buffer(sb, "\\t"); break;
                default:
                    if ((unsigned char)c < 32) {
                        append_buffer(sb, "\\u00%02x", c);
                    } else {
                        append_buffer(sb, "%c", c);
                    }
            }
        }
    }
    append_buffer(sb, "\"");
}
// Execute query 
int
pg_exec_query_no_rslt(
    PGconn *conn, 
    const char *query
    ) 
{
  int status = 0;
  if ( conn == NULL ) { go_BYE(-1); } 
  if ( ( query == NULL ) || ( *query == '\0' ) ) { go_BYE(-1); } 

  PGresult *res = PQexec(conn, query);

  if ( res == NULL ) {
    fprintf(stderr, "ERROR: [%s]\n", PQerrorMessage(conn)); go_BYE(-1); 
  }
  ExecStatusType pqstatus;
  pqstatus = PQresultStatus(res); 
  if ( pqstatus != PGRES_COMMAND_OK) {
    fprintf(stderr, "ERROR: [%s]\n", PQerrorMessage(conn)); go_BYE(-1); 
  }
  /* TODO P2 Should I check num rows affected?
     char *rows_affected_str = PQcmdTuples(res);
     int rows_affected = atoi(rows_affected_str);

     if (rows_affected == 0) { ... }
     */
BYE:
  if ( res != NULL ) { PQclear(res); }
  return status;
}


// Execute query and return result as a JSON string
static char 
*query_to_json(
    PGconn *conn, 
    const char *query,
    int *ptr_n_rows
    ) 
{
  PGresult *res = PQexec(conn, query);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
    PQclear(res);
    return NULL;
  }

  int rows = PQntuples(res);
  *ptr_n_rows = rows; 
  int cols = PQnfields(res);

  StringBuffer sb;
  init_buffer(&sb, 4096);

  append_buffer(&sb, "[");

  for (int i = 0; i < rows; i++) {
    append_buffer(&sb, "%s{", (i == 0) ? "" : ",");

    for (int j = 0; j < cols; j++) {
      const char *col_name = PQfname(res, j);

      append_buffer(&sb, "\"%s\":", col_name);

      if (PQgetisnull(res, i, j)) {
        append_buffer(&sb, "null");
      } else {
        // You can add logic here to check data types using PQftype(res, j)
        // and avoid quotes for numbers/booleans. For simplicity, we 
        // treat all values as strings or format appropriately.
        json_escape_append(&sb, PQgetvalue(res, i, j));
      }

      if (j < cols - 1) {
        append_buffer(&sb, ",");
      }
    }
    append_buffer(&sb, "}");
  }

  append_buffer(&sb, "]");

  PQclear(res);
  return sb.data;
}

int 
pg_get_conn(
    const char * const host,
    const char * const db,
    const char * const user,
    const char * const pass,
    PGconn **ptr_conn
    ) 
{
  int status = 0;
  PGconn *conn = NULL; 
#define CONN_LEN 1024
  char conn_info[CONN_LEN]; memset(conn_info, 0, CONN_LEN);
  // Connection string format for libpq
  *ptr_conn = NULL;
  if ( ( pass == NULL ) || ( *pass == '\0' ) ) { 
  snprintf(conn_info, CONN_LEN-1, 
      "host=%s dbname=%s user=%s ", host, db, user); 
  }
  else {
  snprintf(conn_info, CONN_LEN-1, 
      "host=%s dbname=%s user=%s password=%s", host, db, user, pass);
  }

  conn = PQconnectdb(conn_info);
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Connection to database failed: %s\n", 
        PQerrorMessage(conn));
    go_BYE(-1);
  }
  *ptr_conn = conn;
BYE:
  return status;
}

int 
pg_get_rslt_set(
    PGconn *in_conn,
    const char * const query,
    const char * const host,
    const char * const db,
    const char * const user,
    const char * const pass,
    char **ptr_json_result,
    int *ptr_n_rows
    ) 
{
  int status = 0;
  PGconn *conn = NULL; bool conn_set = false;
  *ptr_n_rows = 0;
  *ptr_json_result = NULL;
  // Connection string format for libpq
  //
  if ( in_conn == NULL ) { 
    status = pg_get_conn(host, db, user, pass, &conn);
    cBYE(status);
    conn_set = true;
  }
  else {
    conn = in_conn;
  }

  *ptr_json_result = query_to_json(conn, query, ptr_n_rows);

  if ( *ptr_json_result == NULL ) {
    go_BYE(-1);
  }
BYE:
  if ( conn_set ) { if ( conn != NULL ) { PQfinish(conn); } }
  return status;
}
