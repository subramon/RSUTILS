#include "q_incs.h"
#include "mk_temp_file.h"

int
mk_temp_file(
    const char * in_tmpl,
    const char * const suffix,
    char **ptr_str
    )
{
  int status = 0;
  char * template = NULL;
  int fd = -1;

  if ( in_tmpl == NULL ) { go_BYE(-1); }
  size_t in_len = strlen(in_tmpl);
  if ( in_len <= 6 ) { go_BYE(-1); }
  if ( strcmp(in_tmpl + (in_len - 6), "XXXXXX") != 0 ) {
    go_BYE(-1);
  }

  if ( suffix == NULL ) {
    template = strdup(in_tmpl);
    if ( template == NULL ) { go_BYE(-1); }
    fd = mkstemp(template); // modified by mkstemp
  }
  else {
    size_t len = in_len + strlen(suffix) + 1;
    template = malloc(len);
    if ( template == NULL ) { go_BYE(-1); }
    int nw = snprintf(template, len, "%s%s", in_tmpl, suffix);
    if ( ( nw < 0 ) || ( (size_t)nw >= len ) ) { go_BYE(-1); }
    fd = mkstemps(template, (int)strlen(suffix)); // modified by mkstemps
  }
  if ( fd < 0 ) { free_if_non_null(template); go_BYE(-1); }
  close(fd); 
  *ptr_str = template;
BYE:
  return status;
}
#undef  TEST 
#ifdef TEST 
// gcc -g mk_temp_file.c -I../inc/ isfile.c
#include "isfile.h"
int
main()
{
  int status = 0;
  const char *template = "/tmp/_XXXXXX";
  const char *suffix = ".csv";
  char *tempf = NULL;
  status = mk_temp_file(template, suffix, &tempf); cBYE(status);
  if ( !isfile(tempf) ) { go_BYE(-1); }
  printf("SUCCESS. Created %s \n", tempf);
  unlink(tempf); 
BYE:
  return status;
}
#endif
