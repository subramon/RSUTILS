// gcc add_time.c  -I/usr/include/mysql/ /usr/lib/x86_64-linux-gnu/libmysqlclient.so
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WHEREAMI { fprintf(stderr, "Line %3d of File %s \n", __LINE__, __FILE__);  }
#define go_BYE(x) { WHEREAMI; status = x ; goto BYE; }
#define cBYE(x) { if ( (x) != 0 ) { go_BYE((x)) } }
#define fclose_if_non_null(x) { if ( (x) != NULL ) { fclose((x)); (x) = NULL; } } 
#define free_if_non_null(x) { if ( (x) != NULL ) { free((x)); (x) = NULL; } }
#define return_if_fopen_failed(fp, file_name, access_mode) { if ( fp == NULL ) { fprintf(stderr, "Unable to open file %s for %s \n", file_name, access_mode); go_BYE(-1); } }
#define return_if_malloc_failed(x) { if ( x == NULL ) { fprintf(stderr, "Unable to allocate memory\n"); go_BYE(-1); } }


#define return_if_mysql_failed(conn) { if ( conn == NULL ) { fprintf(stderr, "%s\n", mysql_error(conn)); go_BYE(-1); } }

#define return_if_mysql_failed(conn) { if ( conn == NULL ) { fprintf(stderr, "%s\n", mysql_error(conn)); go_BYE(-1); } }


static void 
finish_with_error(
      MYSQL *conn
      )
{
  fprintf(stderr, "%s\n", mysql_error(conn));
  mysql_close(conn);
  exit(1);
}

int 
main(
    int argc, 
    char **argv
    )
{
  int status = 0;
  char query[1024]; 
  MYSQL *chk_conn = NULL;
  MYSQL *conn = mysql_init(NULL);
  if ( conn == NULL ) { 
    fprintf(stderr, "%s\n", mysql_error(conn)); go_BYE(-1); 
  }

  chk_conn =  mysql_real_connect(conn, "localhost", "user12", "34klq*",
        "testdb", 0, NULL, 0);
  if ( chk_conn != conn ) { 
    fprintf(stderr, "%s\n", mysql_error(conn)); go_BYE(-1); 
  }

  MYSQL_TIME ts;
  MYSQL_BIND bind[3];
  MYSQL_STMT *stmt = NULL;
  /* CREATE TABLE test_table ( df DATE, dtf DATETIME, tsf TIMESTAMP, 
   * autof TIMESTAMP DEFAULT CURRENT_TIMESTAMP );
   */
  strcpy(query, 
      " INSERT INTO test_table "
      " (df, dtf, tsf) "
      " VALUES(?,?,?) "
      );
  stmt = mysql_stmt_init(conn);
  if ( stmt == NULL ) { 
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    fprintf(stderr, "%s\n", mysql_error(conn)); go_BYE(-1); 
  }
  //--------------------------------------------
  status = mysql_stmt_prepare(stmt, query, strlen(query));
  if ( status != 0 ) { 
    fprintf(stderr, "\n mysql_stmt_prepare(), INSERT failed");
    fprintf(stderr, "\n %s", mysql_stmt_error(stmt));
    exit(0);
  }
  /* set up input buffers for all 3 parameters */
  ts.year= 2002;
  ts.month= 02;
  ts.day= 03;
  ts.hour= 12;
  ts.minute= 34;
  ts.second= 56;

  bind[0].buffer_type= MYSQL_TYPE_DATE;
  bind[0].buffer= (char *)&ts;
  bind[0].is_null= 0;
  bind[0].length= 0;
  bind[1] = bind[2] = bind[0];
    mysql_stmt_bind_param(stmt, bind);
  /* supply the data to be sent in the ts structure */
  status = mysql_stmt_execute(stmt); cBYE(status); 
  /* 
   * mysql> select * from test_table;
+------------+---------------------+---------------------+---------------------+
| df         | dtf                 | tsf                 | autof               |
+------------+---------------------+---------------------+---------------------+
| 2002-02-03 | 2002-02-03 00:00:00 | 2002-02-03 00:00:00 | 2022-02-28 07:08:42 |
+------------+---------------------+---------------------+---------------------+
1 row in set (0.00 sec)
*/
  strcpy(query, "select * from test_table"); 
  status = mysql_real_query(conn, query, strlen(query));
  MYSQL_RES *rslt = mysql_store_result(conn); 
  my_ulonglong n = mysql_num_rows(rslt);
  printf("n = %ld \n", n); 

BYE:
  if ( conn != NULL ) { mysql_close(conn); }
  if ( stmt != NULL ) { mysql_stmt_close(stmt); }
  return status;
}
