#ifndef _CSV_META_H
#define _CSV_META_H

#include "qtypes.h"
//START_FOR_CDEF
typedef struct _csv_meta_t {
  //-- description of data 
  char **col_names; // [nC]
  uint32_t *widths; // [nC]
  char **formats; // [nC]
  qtype_t *qtypes; // [nC]
  bool *has_nulls; // [nC]
  bool *is_load; // [nC]
  uint32_t nC;
  //--------------------------------------------
} csv_meta_t;
//STOP_FOR_CDEF
#endif // _CSV_META_H
