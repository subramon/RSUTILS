#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"

#include "qtypes.h"
#include "num_lines_safe.h"
#include "read_csv.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  const char *infile = "./data/data_1_test_read_csv.csv";
  char *in_X = NULL; 
  size_t in_nX = 0; 
  uint32_t ncols = 10; 

  qtype_t *qtypes  = malloc(ncols * sizeof(qtype_t));
  memset(qtypes, 0, ncols * sizeof(uint32_t));
  uint32_t *widths = malloc(ncols * sizeof(uint32_t));
  memset(widths, 0, ncols * sizeof(uint32_t));
  char ** formats  = malloc(ncols * sizeof(char *));
  memset(formats, 0, ncols * sizeof(char *));
  bool *is_load  = malloc(ncols * sizeof(bool));
  memset(is_load, 0, ncols * sizeof(bool));
  bool *has_nulls  = malloc(ncols * sizeof(bool));
  memset(has_nulls, 0, ncols * sizeof(bool));

  char ** out = NULL;  
  bool ** nn_out= NULL; 

  uint32_t nrows = 0; 
  const char * const str_fld_sep = ",";
  const char * const str_fld_delim = "\"";
  const char * const str_rec_sep = "\n";
  bool is_hdr = true;

  uint32_t max_cell_width = 15;
  status = num_lines_safe(infile, in_X, in_nX, max_cell_width, ncols,
    str_fld_sep, str_rec_sep, &nrows);
  cBYE(status);
  if ( nrows != 6 ) { go_BYE(-1); }
  if ( is_hdr ) { 
    if ( nrows < 2 ) { go_BYE(-1); }
    nrows--;
  }
  else {
    if ( nrows < 1 ) { go_BYE(-1); }
  }

  // set input arguments 
  qtypes[0] = BL;
  qtypes[1] = I1;
  qtypes[2] = I2;
  qtypes[3] = I4;
  qtypes[4] = I8;
  qtypes[5] = F4;
  qtypes[6] = F8;
  qtypes[7] = TM1;
  qtypes[8] = SC;
  qtypes[9] = Q0;

  widths[0] = get_width_c_qtype(BL);
  widths[1] = get_width_c_qtype(I1);
  widths[2] = get_width_c_qtype(I2);
  widths[3] = get_width_c_qtype(I4);
  widths[4] = get_width_c_qtype(I8);
  widths[5] = get_width_c_qtype(F4);
  widths[6] = get_width_c_qtype(F8);
  widths[7] = get_width_c_qtype(TM1);
  widths[8] = 16;
  widths[9] = 0;

  formats[8] = "%Y-%m-%d";

  is_load[9] = false;
  out = malloc(ncols * sizeof(char *));
  memset(out, 0,  ncols * sizeof(char *));
  for ( uint32_t i = 0; i < ncols; i++ ) { 
    if ( i == 9 ) { continue; }
    out[i] = malloc(nrows * widths[i]);
  }

  has_nulls[2] = true;
  nn_out = malloc(ncols * sizeof(char *));
  memset(nn_out, 0, ncols * sizeof(char *));
  nn_out[2] = malloc(nrows * sizeof(bool));

  status = read_csv(
      infile,
      in_X,
      in_nX,
      qtypes, // [ncols] 
      widths, // [ncols] (needed for SC)
      formats, // [ncols] (needed for TM)
      is_load, // [ncols] whether col has nulls
      has_nulls, // [ncols] whether col to be loadde
      out, // [ncols][nrows]
      nn_out, // [ncols][nrows]
      nrows,
      ncols,
      str_fld_sep,
      str_fld_delim,
      str_rec_sep,
      is_hdr
      );
  cBYE(status);
  printf("SUCCESS\n");
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_if_non_null(formats);
  free_if_non_null(is_load);
  free_if_non_null(has_nulls);
  for ( uint32_t i = 0; i < ncols; i++ ) { 
    free_if_non_null(nn_out[i]);
    free_if_non_null(out[i]);
  }
  free_if_non_null(nn_out);
  free_if_non_null(out);
  return status;
}
