#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <regex.h>
#include <dirent.h>
#include "q_macros.h"
#include "free_2d_array.h"
#include "ls.h"

//------------------------------------------------------
static int 
num_dir_entries(
    const char * const dir_name
    )
{
  int status = 0; 
  DIR *d = NULL;
  struct dirent *dir = NULL;
  d = opendir(dir_name); 
  if ( d == NULL ) { 
    fprintf(stderr, "Couldn't open the directory [%s]\n", dir_name);
    go_BYE(-1); 
  }
  int szX = 0;
  while ((dir = readdir(d)) != NULL) {
    szX++;
  }
BYE:
  if ( d != NULL ) { closedir(d); d = NULL; }// Close directory
  if ( status < 0 ) { return -1; } else { return szX; }
}
//------------------------------------------------------
int
ls(
    const char * const dir_name,
    bool is_file,
    bool is_dir,
    const char * const reg_expr,
    char ***ptr_X,
    uint32_t *ptr_nX
  )
{
  int status = 0;
  DIR *d = NULL;
  struct dirent *dir = NULL;
  regex_t regex; memset(&regex, 0, sizeof(regex_t));
  bool is_regex = false;
  int reti;

  char **X = NULL;

  if ( dir_name == NULL ) { go_BYE(-1); }
  if ( ( !is_file ) && ( !is_dir ) ) { go_BYE(-1); }

  if ( reg_expr != NULL ) {
    /* Compile regular expression */
    reti = regcomp(&regex, reg_expr, 0);
    if ( reti ) {
      fprintf(stderr, "Could not compile regex [%s]\n", reg_expr); go_BYE(-1);
    }
    is_regex = true; 
  }
  //-----------------------------------------------------
  int szX = num_dir_entries(dir_name);  
  if ( szX < 0 ) { go_BYE(-1); }
  if ( szX == 0 ) { return status; }
  //-- allocate output 
  X = malloc(szX * sizeof(char *)); // NOTE: over allocation
  memset(X, 0, szX * sizeof(char *));
  // re-read directory
  d = opendir(dir_name);  if ( d == NULL ) { go_BYE(-1); }
  int xidx = 0;
  while ((dir = readdir(d)) != NULL) {
    if ( strcmp(dir->d_name, ".") == 0 ) { continue; }
    if ( strcmp(dir->d_name, "..") == 0 ) { continue; }
    if ( is_regex ) {
      /* Execute regular expression */
      reti = regexec(&regex, dir->d_name, 0, NULL, 0);
      if (!reti) {
        // printf("Match for %s\n ", dir->d_name);
      }
      else if (reti == REG_NOMATCH) {
        // printf("No match for %s\n ", dir->d_name);
        continue; 
      }
      else {
        char msgbuf[128]; 
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        go_BYE(-1); 
      }
    }
    if ( ( is_file ) && ( dir->d_type == DT_REG ) ) {
      if ( xidx >= szX ) { go_BYE(-1); }
      X[xidx++] = strdup(dir->d_name);
    }
    if ( ( is_dir ) && ( dir->d_type == DT_DIR ) ) {
      if ( xidx >= szX ) { go_BYE(-1); }
      X[xidx++] = strdup(dir->d_name);
    }
  }
  *ptr_X  =  X;
  *ptr_nX = xidx;
BYE:
  if ( d != NULL ) { closedir(d); d = NULL; }// Close directory
  if ( is_regex ) { 
    /* Free memory allocated to the pattern buffer by regcomp() */
    regfree(&regex);
  }
  return status;
}

#undef QD_TEST
#ifdef QD_TEST
int
main(
    void
    )
{
  int status = 0;
  char **X = NULL; uint32_t nX = 0;
  status = ls(".", true, false, ".*.c$", &X, &nX); cBYE(status);
  for ( uint32_t i = 0; i <  nX; i++ ) {
    printf("%3u:%s\n", i+1, X[i]); 
  }
  free_2d_array(&X, nX);
BYE:
  return status;
}

#endif // QD_TEST
