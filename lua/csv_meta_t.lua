 return [[ 

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
