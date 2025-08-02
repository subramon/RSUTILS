#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "qtypes.h"
#include "txt_to_F4.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char line[64]; 
  FILE *ifp = NULL; bool is_stdin = false; 
  FILE *ofp = NULL; bool is_stdout = false;
  bool is_hdr = false; 

  if ( argc != 5 ) { go_BYE(-1); }
  const char * const infile    = argv[1];
  const char * const str_qtype = argv[2];
  const char * const outfile   = argv[3];
  const char * const str_is_hdr = argv[4];

  //-------------------------------------------------
  if ( strcmp(infile, "_") == 0 ) { 
    ifp = stdin;
    is_stdin = true; 
  }
  else {
    ifp = fopen(infile, "r");
    return_if_fopen_failed(ifp, infile, "r");
    is_stdin = false; 
  }
  //-------------------------------------------------
  qtype_t qtype = get_c_qtype(str_qtype);
  if ( qtype == Q0 ) { go_BYE(-1); }
  //-------------------------------------------------
  if ( strcmp(outfile, "_") == 0 ) { 
    ofp = stdin;
    is_stdout = true; 
  }
  else {
    ofp = fopen(outfile, "w");
    return_if_fopen_failed(ofp, outfile, "w");
    is_stdin = false; 
    if ( strcmp(infile, outfile) == 0 ) { go_BYE(-1); }
  }
  //-------------------------------------------------
  if ( strcmp(str_is_hdr, "_") == 0 ) { 
    is_hdr = false; 
  }
  else {
    if ( strcasecmp(str_is_hdr, "true") == 0 ) { 
      is_hdr = true; 
    }
    else {
      is_hdr = false; 
    }
  }
  //-------------------------------------------------
  for ( int lno = 0; !feof(ifp); lno++ ) { 
    memset(line, 0, 64);
    char *cptr = fgets(line, 64-1, ifp);
    if ( cptr == NULL ) { break; }
    // skip header line if needed
    if ( ( lno == 0 ) && ( is_hdr ) ) { continue; }
    switch ( qtype ) { 
      case F4 : 
        {
          float ftemp;
          status = txt_to_F4(line, &ftemp); cBYE(status);
          fwrite(&ftemp, sizeof(float), 1, ofp);
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }

BYE:
  if ( !is_stdin  ) { fclose_if_non_null(ifp); }
  if ( !is_stdout ) { fclose_if_non_null(ofp); }
  return status;
}
