#include "q_incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "csv_meta_t.h"
#include "csv_meta_aux.h"

void
csv_meta_free(
    csv_meta_t  *ptr_M
    )
{
  free_2d_array(&(ptr_M->col_names),  ptr_M->nC);
  free_if_non_null(ptr_M->widths);
  free_2d_array(&(ptr_M->formats),  ptr_M->nC);
  free_if_non_null(ptr_M->qtypes);
  free_if_non_null(ptr_M->has_nulls);
  free_if_non_null(ptr_M->is_load);
}
