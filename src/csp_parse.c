#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "q_macros.h"
#include "csp_info.h"

#include "file_as_str.h"
#include "extract_name_value.h"
#include "trim.h"
#include "str_between.h"
#include "csp_parse.h"

// Parsing is very brittle TODO P3
int
csp_parse(
    const char * const csp_file,
    csp_info_t *ptr_csp_info

    )
{
  int status = 0;
  char *str = NULL, *bak_str = NULL; size_t len = 0, bak_len = 0;

  if ( csp_file == NULL ) { go_BYE(-1); }
  // allocate space to store fragments 
  ptr_csp_info->sz = 1024; // over-allocation  TODO P3 
  csp_frag_t *l_csp_frag = malloc(ptr_csp_info->sz * sizeof(csp_frag_t));
  memset(l_csp_frag, 0, ptr_csp_info->sz * sizeof(csp_frag_t));
  char **l_frag_label = malloc(ptr_csp_info->sz * sizeof(char *));
  memset(l_frag_label, 0, ptr_csp_info->sz * sizeof(char *));

  // read in file to be fragmented 
  status = file_as_str(csp_file, &str, &len); cBYE(status);
  bak_str = str;
  bak_len = len;
  int frag_idx = 0;
  for ( ; ; ) { 
    char *cptr = strstr(str, "<?Lua START"); 
    if ( cptr == NULL ) {
      // Put everything from str to EOF in a static string 
      l_csp_frag[frag_idx].csp_src = csp_static;
      size_t llen = len;
      char *lstr = malloc(llen+1);
      memset(lstr, 0,  (llen+1));
      strncpy(lstr, str, llen);
      //----
      l_csp_frag[frag_idx].csp_src = csp_static;
      l_csp_frag[frag_idx].frag.static_str.str = lstr;
      l_csp_frag[frag_idx].frag.static_str.len = llen;
      frag_idx++;
      break; // all done 
    }
    else {
      // Put everything from str to cptr in a static string 
      /* everything goes into static str */
      size_t llen = cptr - str; 
      char *lstr = malloc(llen+1);
      memset(lstr, 0,  (llen+1));
      strncpy(lstr, str, llen);
      //----
      l_csp_frag[frag_idx].csp_src = csp_static;
      l_csp_frag[frag_idx].frag.static_str.str = lstr;
      l_csp_frag[frag_idx].frag.static_str.len = llen;
      frag_idx++;
      //--- advance pointers
      str += llen;
      len -= llen;
      // capture the function name and the arguments 
      lstr = NULL; llen = 0; 
      char *tmp_str = NULL;
      char *xptr = strstr(str, "?>"); 
      if ( xptr == NULL ) { go_BYE(-1); }
      llen = xptr - str; 
      tmp_str = malloc(llen+1);
      memset(tmp_str, 0,  (llen+1));
      strncpy(tmp_str, str, llen);
      //-- Now we need to break tmp_str into func and args and label 
      // --------------------------------------------------
      char *func = NULL; 
      status = str_between(tmp_str, "FUNC=[", "]", &func); cBYE(status);
      if ( func == NULL ) { go_BYE(-1); }

      char *label = NULL; 
      status = str_between(tmp_str, "LABEL=[", "]", &label); cBYE(status);
      if ( label == NULL ) { go_BYE(-1); }

      char *tmp_args = NULL; char *args = NULL; 
      status = str_between(tmp_str, "ARGS ", "?>", &tmp_args); cBYE(status);
      // okay for args to be null 
      if ( tmp_args != NULL ) { 
        args = strdup(tmp_args);
        status = trim(args, tmp_args, strlen(args)+1); cBYE(status);
        if ( *args == '\0' )  { free_if_non_null(args); }
      }
      free_if_non_null(tmp_args);

      free_if_non_null(tmp_str);
      // START capture the dummy section if any 
      char *dummy = NULL, *tmp_dummy = NULL;
      char *yptr = xptr + strlen("?>");
      char *zptr = strstr(yptr, "<?Lua STOP"); 
      size_t dlen = zptr - yptr;
      tmp_dummy = malloc(dlen+1); memset(tmp_dummy, 0, dlen+1);
      dummy = malloc(dlen+1); memset(dummy, 0, dlen+1);
      strncpy(tmp_dummy, yptr, dlen);
      status = trim(tmp_dummy, dummy, dlen+1);
      if ( *dummy == '\0' )  { free_if_non_null(dummy); }
      free_if_non_null(tmp_dummy);
      // STOP  capture the dummy section if any 
      l_csp_frag[frag_idx].csp_src = csp_Lua_func;
      l_csp_frag[frag_idx].frag.Lua_str.func = func; func = NULL; 
      l_csp_frag[frag_idx].frag.Lua_str.args = args; args = NULL; 
      l_csp_frag[frag_idx].frag.Lua_str.dummy = dummy; dummy = NULL; 
      l_frag_label[frag_idx] = label; label = NULL; 
      frag_idx++;
      // skip over the dynamic section
      
      xptr = strstr(cptr, "<?Lua STOP"); 
      if ( xptr == NULL ) { go_BYE(-1); }
      yptr = strstr(xptr, "?>"); 
      if ( yptr == NULL ) { go_BYE(-1); }
      // advance pointers 
      str = yptr + strlen("?>");
      len -= (str - cptr);
    }
  }
  ptr_csp_info->n = frag_idx;
  ptr_csp_info->csp_frag = l_csp_frag;
  ptr_csp_info->frag_label = l_frag_label;

  for ( int i = 0; i < ptr_csp_info->n; i++ ) { 
    if ( l_csp_frag[i].csp_src == csp_Lua_func ) { 
      printf("%d:label=%s\nfunc=%s\nargs=\t%s\ndummy=\t%s\n", i,
      l_frag_label[i], 
        l_csp_frag[i].frag.Lua_str.func,
        l_csp_frag[i].frag.Lua_str.args,
        l_csp_frag[i].frag.Lua_str.dummy);
    }
  }

BYE:
  free_if_non_null(bak_str);
  return status;
}
