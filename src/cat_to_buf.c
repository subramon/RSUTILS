#include "q_incs.h"
#include "multiple.h"
#include "cat_to_buf.h"

#define MIN_TO_ADD 64
// concatenates str to buf 
// If buf is not big enough, it will re-allocate space
int
cat_to_buf(
    char **ptr_buf,  // [bufsz]
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen, 
    const char * const str,
    int str_len
    )
{
  int status = 0;

  char *buf  = *ptr_buf;
  uint32_t bufsz  = *ptr_bufsz; 
  uint32_t buflen = *ptr_buflen; 

  if ( str == NULL ) { go_BYE(-1); } 
  if ( str_len == 0  ) {  str_len  = strlen(str); }
  if ( str_len == 0  ) {  return status; } // nothing to do 

  if ( *str == '\0' ) { go_BYE(-1); }
  if ( buf == NULL ) { 
    if ( bufsz != 0 ) { go_BYE(-1); } 
    if ( buflen != 0 ) { go_BYE(-1); } 
    buf = malloc(MIN_TO_ADD);
    return_if_malloc_failed(buf);
    memset(buf, 0, MIN_TO_ADD);
    bufsz = MIN_TO_ADD;
  }
  if ( (str_len + buflen)  > (bufsz-1) ) { // +1 for nullc
    uint32_t n1 = str_len + buflen +1; 
    uint32_t n2 = str_len + bufsz; 
    bufsz = mcr_max(n1, n2); 
    bufsz = multiple_n(bufsz, 8);
    char *newbuf = malloc(bufsz);
    return_if_malloc_failed(newbuf);
    memset(newbuf, 0, bufsz); 
    if ( buf != NULL ) { 
      memcpy(newbuf, buf, buflen); 
      if ( newbuf[bufsz-1] != '\0' ) { go_BYE(-1); }
      if ( newbuf[buflen] != '\0' ) { go_BYE(-1); }
    }
    free_if_non_null(buf); 
    buf = newbuf;
  }
#ifdef DEBUG
  if ( buflen != strlen(buf) ) { 
    uint32_t len = strlen(buf); 
    WHEREAMI;
  }
#endif
  memcpy(buf + buflen, str, str_len);
  buflen += str_len;
#ifdef DEBUG
  if ( buflen != strlen(buf) ) { 
    uint32_t len = strlen(buf); 
    WHEREAMI;
  }
#endif
  if ( buf[buflen] != '\0' ) { go_BYE(-1); } 
  if ( buf[bufsz-1] != '\0' ) { go_BYE(-1); }
  //---------------------------------------
  *ptr_buf    = buf;
  *ptr_bufsz  = bufsz;
  *ptr_buflen = buflen;
BYE:
  return status;
}
