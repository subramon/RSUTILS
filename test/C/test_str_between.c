#include "q_incs.h"
#include "str_between.h"

int
main()
{
  int status = 0;
  int n = 32;
  char *out = NULL; 

  char *eg1 = "abcdefghi";
  status = str_between(eg1, "abc", "ghi", &out);
  if ( out == NULL ) { go_BYE(-1); }
  if ( strcmp(out, "def") != 0 ) { go_BYE(-1); }
  free_if_non_null(out);

  char *eg2 = "XabcdefghiY";
  status = str_between(eg2, "abc", "ghi", &out);
  if ( out == NULL ) { go_BYE(-1); }
  if ( strcmp(out, "def") != 0 ) { go_BYE(-1); }
  free_if_non_null(out);

  char *eg3 = "XabcdefghYi";
  status = str_between(eg3, "abc", "ghi", &out);
  if ( out != NULL ) { go_BYE(-1); }

  char *eg4 = "XbcdefghiY";
  status = str_between(eg4, "abc", "ghi", &out);
  if ( out != NULL ) { go_BYE(-1); }

  printf("SUCCESS\n");
BYE:
  return status;
}
