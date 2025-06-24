#include "q_incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "csv_spec.h"
#include "csv_spec_aux.h"

void
csv_spec_free(
    csv_spec_t  *ptr_S
    )
{
  free_2d_array(&(ptr_S->break_cols),  ptr_S->nB);
  free_if_non_null(ptr_S->str_fld_sep);
  free_if_non_null(ptr_S->str_fld_delim);
  free_if_non_null(ptr_S->str_rec_sep);
}
