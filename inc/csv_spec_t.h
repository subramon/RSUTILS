#ifndef RSUTILS_INC_CSV_SPEC_T_H
#define RSUTILS_INC_CSV_SPEC_T_H

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
#endif // RSUTILS_INC_CSV_SPEC_T_H
