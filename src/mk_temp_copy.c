#include "q_incs.h"
#include "copy_file.h"
#include "mk_temp_copy.h"

int
mk_temp_copy( 
    const char *infile,
    const char * in_tmpl,
    const char * const suffix,
    char **ptr_outfile
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
  *ptr_outfile = template;
  status = copy_file(infile, *ptr_outfile); cBYE(status);
BYE:
  return status;
}
#undef  TEST 
#ifdef TEST 
// gcc -g mk_temp_copy.c -I../inc/ isfile.c
#include "isfile.h"
int
main()
{
  int status = 0;
  const char *template = "/tmp/_XXXXXX";
  const char *suffix = ".csv";
  char *tempf = NULL;
  status = mk_temp_copy("./copy_file.c", template, suffix, &tempf); 
  cBYE(status);
  if ( !isfile(tempf) ) { go_BYE(-1); }
  printf("SUCCESS. Created %s \n", tempf);
  // unlink(tempf); 
BYE:
  free_if_non_null(tempf); 
  return status;
}
#endif
