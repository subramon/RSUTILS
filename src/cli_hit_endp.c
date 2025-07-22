#include <curl/curl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "hit_endp.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *retval = NULL;
  if ( argc != 4 ) { go_BYE(-1); }
  const char * const protocol = "http";
  const char * const server = argv[1]; 
  uint16_t port = (uint16_t)atoi(argv[2]);
  const char * const api = argv[3]; 


  status = hit_endp(
    protocol,
    server,
    port,
    api,
    &retval
    );
  if ( retval != NULL ) { 
    printf("%s\n", retval);
  }
BYE:
  free_if_non_null(retval);
  return status;
}
