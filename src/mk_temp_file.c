#include "q_incs.h"
#include "mk_temp_file.h"

char *
mk_temp_file(
    const char * in_tmpl,
    const char * const suffix
    )
{
  int status = 0;
  char * template = NULL;
  int fd = -1;

  size_t in_len = strlen(in_tmpl);
  if ( in_len <= 6 ) { go_BYE(-1); }
  if ( strcmp(in_tmpl+ (in_len-6), "XXXXXX") != 0 ) { 
    go_BYE(-1);
  }

  if ( in_tmpl == NULL )  {
    template = "/tmp/_temp_file_XXXXXX";
  }
  else {
    if ( suffix == NULL ) { 
      template = strdup(in_tmpl);
      fd =  mkstemp(template); // modified by mkstemp
    }
    else {
      size_t len = strlen(in_tmpl) + strlen(suffix) + 1; 
      template = malloc(len);
      sprintf(template, "%s%s", in_tmpl, suffix);
      fd =  mkstemps(template, strlen(suffix)); // modified by mkstemps
    }
  }
  if ( fd < 0 ) { go_BYE(-1); }
  close(fd); 
BYE:
  if ( status < 0 ) { return NULL; } else { return template; }
}
#undef TEST 
#ifdef TEST 
// gcc -g mk_temp_file.c -I../inc/ isfile.c
#include "isfile.h"
int
main()
{
  int status = 0;
  const char *template = "/tmp/_XXXXXX";
  const char *suffix = ".csv";
  char *tempf = mk_temp_file(template, suffix);
  if ( !isfile(tempf) ) { go_BYE(-1); }
  printf("SUCCESS\n");
BYE:
  return status;
}
#endif
