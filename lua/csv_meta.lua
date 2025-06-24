 return [[ 

typedef struct _csv_spec_t {
  char *str_fld_sep;
  char *str_fld_delim;
  char *str_rec_sep;
  char **break_cols;
  uint32_t nB;
} csv_spec_t;

typedef struct _csv_meta_t {

  char **col_names;
  uint32_t *widths;
  char **formats;
  qtype_t *qtypes;
  bool *has_nulls;
  bool *is_load;
  uint32_t nC;

} csv_meta_t;

 ]] 
