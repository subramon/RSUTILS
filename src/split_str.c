#include "q_incs.h"
#include "split_str.h"
int
split_str(
    const char * const in_str,
    // restriction: sep is a single characttr string 
    const char * const sep, // separator 
    char ***ptr_outs, // [n_outs]
    uint32_t *ptr_n_outs
    )
{
  int status = 0;
  char **outs = NULL;
  uint32_t n_outs = 0;
  char *l_in_str = NULL;
  // START: Get number of input files and their number n_outs 
  if ( ( in_str == NULL ) || ( *in_str == '\0' ) ) { go_BYE(-1); } 
  if ( ( sep == NULL ) || ( *sep == '\0' ) ) { go_BYE(-1); } 
  if ( strlen(sep) > 1 ) { go_BYE(-1); }
  char c_sep = sep[0];
  n_outs = 1;
  for ( uint32_t i = 0; ; i++ ) { 
    if ( in_str[i] == '\0' ) { break; }
    if ( in_str[i] == c_sep ) { n_outs++; }
  }
  outs = malloc(n_outs * sizeof(char *));
  return_if_malloc_failed(outs);
  memset(outs, 0,  n_outs * sizeof(char *));

  // following beecause strtok is messing up in_str
  l_in_str = strdup(in_str);
  return_if_malloc_failed(l_in_str);
  for ( uint32_t i = 0; i < n_outs; i++ ) {
    char *cptr;
    if ( i == 0 ) { 
      cptr = strtok(l_in_str, sep);
    }
    else {
      cptr = strtok(NULL, sep);
    }
    if ( *cptr == '\0' ) { go_BYE(-1); }
    outs[i] = strdup(cptr);
  }
BYE:
  free_if_non_null(l_in_str);
  *ptr_outs = outs;
  *ptr_n_outs = n_outs;
  return status;
}

// same as above but nullc used as separator
// hence, n_keys must be known 
int
split_str_nullc_sep(
    char * const in_str, // INPUT 
    uint32_t len_in_str, // INPUT optional
    uint32_t n_outs, // INPUT 
    char ***ptr_outs // [n_outs] OUTPUT 
    )
{
  int status = 0;
  char **outs = NULL;
  
  if ( ( in_str == NULL ) || ( *in_str == '\0' ) ) { go_BYE(-1); } 

  outs = malloc(n_outs * sizeof(char *));
  return_if_malloc_failed(outs);
  memset(outs, 0,  n_outs * sizeof(char *));

  char * cptr = in_str;
  uint32_t offset = 0;
  for ( uint32_t i = 0; i < n_outs; i++ ) { 
    uint32_t len = strlen(cptr);
    outs[i] = strdup(cptr);
    cptr += (len+1);
    // to make sure you have not gone too far down in_str
    offset += (len+1);
    if ( len_in_str > 0 ) {
      if ( offset > len_in_str ) { go_BYE(-1); }
    }
  }
  *ptr_outs = outs;
BYE:
  return status;
}
