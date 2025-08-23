/* 
 * Input: ascii file 
 Assumptions: 
 (o) each line contains one numeric value 
 (o) no null values
 (o) no field separators such as double quote
Output: binary file 
  If input file has n lines and sizeof(type) == m, then
  size of output file = n*m bytes
Arguments
(a) name of input file. If empty string provided, then read from stdin
(b) type of numeric value e.g. I2, I4, ...
(c) name of output file. If empty string provided, then write to stdout
(d) true|false -> whether input has a header line that is to be ignored
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "q_macros.h"
#include "qtypes.h"
#include "txt_to_F4.h"
#include "txt_to_I4.h"

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
    is_stdout = false;
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
      case I4 :
        {
          int32_t itemp;
          status = txt_to_I4(line, &itemp); cBYE(status);
          fwrite(&itemp, sizeof(int32_t), 1, ofp);
        }
        break;
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
