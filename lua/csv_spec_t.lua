 return [[ 

typedef struct _csv_spec_t {
  bool is_hdr;
  char *str_fld_sep;
  char *str_fld_delim;
  char *str_rec_sep;
  char **break_cols;
  uint32_t nB;
} csv_spec_t;

 ]] 
