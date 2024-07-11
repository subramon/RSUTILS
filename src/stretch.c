//START_INCLUDES
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "q_macros.h"
#include "rs_mmap.h"
#include "isfile.h"
#include "get_file_size.h"
//STOP_INCLUDES
#include "stretch.h"

//START_FUNC_DECL
int
stretch(
    const char * const file_name,
    uint64_t new_file_size
    ) 
//STOP_FUNC_DECL
{ 
  int status = 0;
  int fd = -1;
  if ( file_name == NULL ) { go_BYE(-1); } 
  if ( !isfile(file_name) ) { go_BYE(-1); } 
  int64_t old_file_size = get_file_size(file_name);
  if ( old_file_size < 0 ) { go_BYE(-1); } 
  if ( new_file_size ==  0) { go_BYE(-1); } 
  if ( (uint64_t)old_file_size > new_file_size ) { go_BYE(-1); } 
  if ( (uint64_t)old_file_size == new_file_size ) { return status; } // nothing to do 

  fd = open(file_name, O_RDWR);
  if ( fd < 0 ) { go_BYE(-1); }
  // o2 = lseek(fd, 0, SEEK_END);
  off_t offset = lseek(fd, new_file_size, SEEK_CUR);
  if ( offset < 0 ) { go_BYE(-1); } 
  // reset to start of file 
  offset = lseek(fd, old_file_size, SEEK_SET);
  // pad with 0's
  // this loop seems awfully slow. 
  for ( uint64_t i = 0; i < new_file_size - (uint64_t)old_file_size; i++ ) {
    char c = 0; 
    write(fd, &c, 1);
  }
  close(fd);
#ifdef DEBUG
  int64_t chk_file_size = get_file_size(file_name); 
  if ( chk_file_size < 0 ) { go_BYE(-1); } 
  if ( (uint64_t)chk_file_size != new_file_size ) { go_BYE(-1); }
#endif

BYE:
  if ( fd >= 0 ) { close(fd); fd = -1; } 
  return status;
}
