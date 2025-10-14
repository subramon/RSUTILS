#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "q_macros.h"
#include "csp_info.h"

#include "rs_mmap.h"
#include "file_as_str.h"
#include "extract_name_value.h"
#include "trim.h"
#include "csp_parse.h"

// Parsing is very brittle TODO P3
int
csp_parse(
    const char * const csp_file,
    csp_info_t *ptr_csp_info

    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  char *str = NULL, *bak_str = NULL; size_t len = 0, bak_len = 0;
  size_t loc = 0;

  // allocate space to store fragments 
  ptr_csp_info->sz = 1024; // over-allocation  TODO P3 
  csp_frag_t *l_csp_frag = malloc(ptr_csp_info->sz * sizeof(csp_frag_t));
  memset(l_csp_frag, 0, ptr_csp_info->sz * sizeof(csp_frag_t));

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
      //-- Now we need to break tmp_str into func and args 
      char *func = NULL; char *args = NULL; 
      char inbuf[32]; memset(inbuf, 0, 32);
      char opbuf[32]; memset(opbuf, 0, 32);
      char *zstr = strstr(tmp_str, "START"); 
      if ( zstr == NULL ) { go_BYE(-1); }
      status = extract_name_value(zstr, "START", '(', inbuf, 32-1);
      cBYE(status);
      if ( *inbuf == '\0' ) { go_BYE(-1); }
      status = trim(inbuf, opbuf, 32); cBYE(status);
      if ( *opbuf == '\0' ) { go_BYE(-1); }
      func = strdup(opbuf);

      char *wstr = strstr(zstr, "()");
      if ( wstr == NULL ) { 
        go_BYE(-1); }
      
      char *vstr = wstr + strlen("()");
      size_t vlen = strlen(vstr);
      char *ustr = malloc(vlen+1);
      memset(ustr, 0, vlen+1);
      status = trim(vstr, ustr, vlen+1); cBYE(status);

      if ( *ustr != '\0' ) {
        args = strdup(ustr);
      }
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
      if ( *dummy == '\0' )  {
        free_if_non_null(dummy);
      }
      free_if_non_null(tmp_dummy);
      // STOP  capture the dummy section if any 
      l_csp_frag[frag_idx].csp_src = csp_Lua_func;
      l_csp_frag[frag_idx].frag.Lua_str.func = func;
      l_csp_frag[frag_idx].frag.Lua_str.args = args;
      l_csp_frag[frag_idx].frag.Lua_str.dummy = dummy;
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
  for ( int i = 0; i < ptr_csp_info->n; i++ ) { 
    if ( l_csp_frag[i].csp_src == csp_Lua_func ) { 
      printf("%d:%s\nargs=\t%s\ndummy=\t%s\n", i,
        l_csp_frag[i].frag.Lua_str.func,
        l_csp_frag[i].frag.Lua_str.args,
        l_csp_frag[i].frag.Lua_str.dummy);
    }
  }
BYE:
  return status;
}
