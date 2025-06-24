#ifndef _CSV_SPEC_H
#define _CSV_SPEC_H

//START_FOR_CDEF
typedef struct _csv_spec_t {
  bool is_hdr; 
  char *str_fld_sep;
  char *str_fld_delim;
  char *str_rec_sep;
  char **break_cols; // [nB] 
  uint32_t nB; 
} csv_spec_t;
//STOP_FOR_CDEF
#endif // _CSV_SPEC_H
