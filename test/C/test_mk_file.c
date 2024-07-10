#include "q_incs.h"
#include <inttypes.h>
#include "q_macros.h"
#include "get_file_size.h"
#include "get_time_usec.h"
#include "mk_dir_file_name.h"
#include "mk_file.h"

#define N 4 
int
main(
    int argc,
    char **argv
    ) 
{ 
  int status = 0;
  char *full_name = NULL;
  char file_name[32];
  sprintf(file_name, "_%" PRIu64 "_", get_time_usec());
  size_t file_size = 1048576; 
  status = mk_file("/tmp/", file_name, file_size); cBYE(status);
  //------
  full_name = mk_dir_file_name("/tmp", file_name);
  int64_t chk_file_size = get_file_size(full_name); 
  if ( chk_file_size != file_size ) { go_BYE(-1); } 

  printf("Successfully completed %s \n", argv[0]); 
BYE:
  if ( full_name != NULL ) { unlink(full_name); } 
  free_if_non_null(full_name); 
  return status;
}
