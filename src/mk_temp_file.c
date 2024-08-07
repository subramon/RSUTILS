#include "q_incs.h"
#include "mk_temp_file.h"

char *
mk_temp_file(
    const char * in_tmpl
    )
{
  int status = 0;
  const char * template = NULL;
  if ( in_tmpl == NULL )  {
    template = "/tmp/_dataset_XXXXXX";
  }
  else {
    template = in_tmpl;
  }
  char *file_name = NULL; 
  file_name = malloc(strlen(template) + 1);
  strcpy(file_name, template);
  int fd =  mkstemp(file_name); 
  if ( fd < 0 ) { go_BYE(-1); }
  close(fd); 
BYE:
  if ( status < 0 ) { return NULL; } else { return file_name; }
}
