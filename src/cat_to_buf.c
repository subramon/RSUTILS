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

  if ( *str == '\0' ) {
    printf("helo woorld\n"); go_BYE(-1);
  }
  if ( buf == NULL ) { 
    if ( bufsz != 0 ) { go_BYE(-1); } 
    if ( buflen != 0 ) { go_BYE(-1); } 
    buf = malloc(MIN_TO_ADD);
    return_if_malloc_failed(buf);
    memset(buf, 0, MIN_TO_ADD);
    bufsz = MIN_TO_ADD;
  }
  if ( str_len + buflen + 1 >= bufsz ) { // +1 for nullc
    bufsz += mcr_max(MIN_TO_ADD, ((str_len + buflen + 1) - bufsz));
    bufsz = multiple_n(bufsz, 8);
    char *newbuf = malloc(bufsz);
    return_if_malloc_failed(newbuf);
    memset(newbuf, 0, bufsz); 
    if ( buf != NULL ) { 
      memcpy(newbuf, buf, buflen); 
    }
    free_if_non_null(buf); 
    buf = newbuf;
  }
  memcpy(buf + buflen, str, str_len);
  buflen += str_len;
  if ( buf[buflen] != '\0' ) { go_BYE(-1); } 
  //---------------------------------------
  *ptr_buf    = buf;
  *ptr_bufsz  = bufsz;
  *ptr_buflen = buflen;
BYE:
  return status;
}
