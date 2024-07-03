//START_INCLUDES
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "q_macros.h"
//STOP_INCLUDES
#include "get_file_size.h"
//START_FUNC_DECL
int64_t 
get_file_size(
	const char * const file_name
	)
//STOP_FUNC_DECL
{
  int status = 0;
  int64_t file_size = -1;
  int fd = -1;
  struct stat filestat;

  fd = open(file_name, O_RDONLY);
  if ( fd < 0 ) { go_BYE(-1); }
  status = fstat(fd, &filestat);  cBYE(status);
  file_size = (int64_t) filestat.st_size;
BYE:
  if ( fd >= 0 ) { close(fd); }
  return file_size;
}
