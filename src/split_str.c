#include "q_incs.h"
#include "split_str.h"
// Note a very smart implementation but will have to do for now 
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
  uint32_t *lens = NULL; // [n_outs]
  uint32_t *offs = NULL; // [n_outs]
  // START: Get number of input files and their number n_outs 
  if ( ( in_str == NULL ) || ( *in_str == '\0' ) ) { go_BYE(-1); } 
  if ( ( sep == NULL ) || ( *sep == '\0' ) ) { go_BYE(-1); } 
  if ( strlen(sep) > 1 ) { go_BYE(-1); }
  char c_sep = sep[0];
  //---------------------------------------------
  // Find out how many strings we will create 
  n_outs = 1;
  for ( uint32_t i = 0; ; i++ ) { 
    if ( in_str[i] == '\0' ) { break; }
    if ( in_str[i] == c_sep ) { n_outs++; }
  }
  // Allocate space for output 
  outs = malloc(n_outs * sizeof(char *));
  return_if_malloc_failed(outs);
  memset(outs, 0,  n_outs * sizeof(char *));

  lens = malloc(n_outs * sizeof(uint32_t));
  return_if_malloc_failed(lens);
  memset(lens, 0,  n_outs * sizeof(uint32_t));

  offs = malloc(n_outs * sizeof(uint32_t));
  return_if_malloc_failed(offs);
  memset(offs, 0,  n_outs * sizeof(uint32_t));
  // strtok does not return a null string for 2nd cell in
  // in_str = "10038493,,1.0,3.0,2019-06-16\n"
  // See Gemini at end 

  uint32_t len = 0;
  uint32_t idx = 0;
  offs[idx] = 0; 
  for ( int i = 0; ; i++ ) { 
    if ( in_str[i] == '\0' ) { 
      lens[idx] = len;
      break;
    }
    if ( in_str[i] == c_sep ) { 
      if ( idx >= n_outs ) { go_BYE(-1); }
      lens[idx] = len;
      len = 0;
      if ( idx < n_outs ) { 
        offs[idx+1] = i+1;
      }
      idx++;
    }
    else {
    len++;
    }
  }
  for ( uint32_t i = 0; i < n_outs; i++ ) { 
    outs[i] = malloc(lens[i] + 1);
    memset(outs[i], 0, lens[i] + 1);
    memcpy(outs[i], in_str+offs[i], lens[i]);
  }
BYE:
  free_if_non_null(lens);
  free_if_non_null(offs);
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

/*
 *
When strtok encounters consecutive delimiters, it treats them as a single delimiter. It does not return empty tokens between them.
For example, if you have the string "apple,,banana,orange" and use ',' as the delimiter, strtok will return "apple", then "banana", then "orange". It will not return an empty string between "apple" and "banana" due to the double comma.
This behavior is useful when parsing data where multiple delimiters should be treated as a single separator, such as multiple spaces between words. However, it means strtok cannot be used to detect and extract empty tokens that might exist between consecutive delimiters in certain data formats. If you need to handle empty tokens, alternative string parsing methods or manual character-by-character scanning would be necessary.
*/
