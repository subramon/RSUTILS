#include "q_incs.h"
#include "web_struct.h"
#include "free_web_info.h"
#include "free_2d_array.h"

int 
free_web_info(
    web_info_t *ptr_W
    )
{
  int status = 0;
  if ( ptr_W == NULL ) { go_BYE(-1); }
  free_2d_array(&(ptr_W->users), ptr_W->n_users);
  free_if_non_null(ptr_W->docroot);
  memset(ptr_W, 0, sizeof(web_info_t));
BYE:
  return status;
}
