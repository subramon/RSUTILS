#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"

// read upto (and including) newline character
// from inbuf into outbuf
// error if outbuf threatens to overflow
// terminate early if no more from inbuf
int
read_to_eoln(
    const char * const inbuf,
    size_t inbuf_len,
    char *outbuf,
    size_t outbuf_sz,
    size_t *ptr_outbuf_len
    )
{
  int status = 0;
  *ptr_outbuf_len = 0;
  if ( inbuf == NULL ) { go_BYE(-1); }
  if ( outbuf == NULL ) { go_BYE(-1); }
  size_t len = 0;
  for ( uint32_t i = 0; i < inbuf_len; i++ ) {
    if ( *inbuf == '\n' ) {
      break;
    }
    if ( len >= outbuf_sz ) { go_BYE(-1); }
    outbuf[len] = *inbuf;
  }
  *ptr_outbuf_len = len;
BYE:
  return status;
}
