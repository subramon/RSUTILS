//START_INCLUDES
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "q_macros.h"
//STOP_INCLUDES
#include "isdir.h"

//START_FUNC_DECL
bool 
isdir (
    const char * const dirname
    )
//STOP_FUNC_DECL
{
  struct stat buf;
  if ( ( dirname == NULL ) || ( *dirname == '\0' ) ) { return false; }
  int status = stat(dirname, &buf );
  if ( ( status == 0 ) && ( S_ISDIR( buf.st_mode ) ) ) { /* Path found, check for directory */
    return true;
  }
  else {
    return false;
  }
}
