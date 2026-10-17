#include <stdio.h>
#include "q_macros.h"
#include "pg_get_rslt_set.h"

int 
main() 
{
  int status = 0;
  char *json_rslt_str = NULL;

  status = pg_get_rslt_set("SELECT * FROM test", "localhost", "test_manager", "subramon", "P123456", &json_rslt_str);
  cBYE(status);
  printf("Result set = \n%s\n", json_rslt_str);
BYE:
  free_if_non_null(json_rslt_str);
}
