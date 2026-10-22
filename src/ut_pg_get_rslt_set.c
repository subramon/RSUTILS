#include <stdio.h>
#include <stdbool.h>
#include "q_macros.h"
#include "pg_get_rslt_set.h"

int 
main() 
{
  int status = 0;
  char *json_rslt_str = NULL;
  PGconn *conn = NULL; bool conn_set = false;
#define CONN_LEN 1024
  char conn_info[CONN_LEN];

  // test without connection
  int n_rows; 
  status = pg_get_rslt_set(NULL, "SELECT * FROM test", "localhost", 
      "test_manager", "subramon", "P123456", &json_rslt_str, &n_rows);
  cBYE(status);
  printf("Result set = \n%s\n", json_rslt_str);
  free_if_non_null(json_rslt_str);
  // test without connection
  memset(conn_info, 0, CONN_LEN);
  const char *host = "localhost";
  const char *db = "test_manager";
  const char *user = "subramon";
  const char *pass = NULL; // "P123456"; Comes from ~/.pgpass
  snprintf(conn_info, CONN_LEN-1, 
      "host=%s dbname=%s user=%s ", host, db, user, pass);
  printf("%s\n", conn_info);

  conn = PQconnectdb(conn_info);
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Connection to database failed: %s\n", 
        PQerrorMessage(conn));
    go_BYE(-1);
  }
  conn_set = true;
  status = pg_get_rslt_set(conn, "SELECT * FROM test", "localhost", 
      "test_manager", "subramon", "P123456", &json_rslt_str, &n_rows);
  cBYE(status);
  printf("SUCCESS\n");
BYE:
  free_if_non_null(json_rslt_str);
  if ( conn_set ) { if ( conn != NULL ) { PQfinish(conn); } }
}
