#include "q_incs.h"
#include "trim.h"

typedef _my_row_t {
  int8_t  x1;
  int16_t x2;
  int32_t x3;
  int64_t x4;
  float   x5;
  double  x6;
} my_row_t;

static int
pr_row(
    uint32_t i,
    my_row_t *r
    )
{
  printf("%u,", i);
  printf("%d,", row->x2);
  printf("%d,", row->x3);
  printf("%d,", row->x1);
  printf("%" PRI64 ",", row->x4);
  printf("%f,",  row->x5);
  printf("%lf\n", row->x6);
}
int
main()
{
  int status = 0;
  uint32_t n_cols = 6;
  uint32_t n_rows = 3; 
  char **cols = NULL; 
  my_row_t *out = NULL;
  my_row_t row; memset(&row, 0, sizeof(my_row_t));
  int8_t *colI1 = malloc(n_rows * sizeof(int8_t));
  int8_t *colI2 = malloc(n_rows * sizeof(int16_t));
  int8_t *colI4 = malloc(n_rows * sizeof(int32_t));
  int8_t *colI8 = malloc(n_rows * sizeof(int64_t));
  int8_t *colF8 = malloc(n_rows * sizeof(float));
  int8_t *colF8 = malloc(n_rows * sizeof(double));

  uint32_t *col_width = malloc(n_cols * sizeof(uint32_t));
  col_width[0] = sizeof(int8_t);
  col_width[1] = sizeof(int16_t);
  col_width[2] = sizeof(int32_t);
  col_width[3] = sizeof(int64_t);
  col_width[4] = sizeof(float);
  col_width[5] = sizeof(double);

  uint32_t *col_offset = malloc(n_cols * sizeof(uint32_t));
  col_offset[0] = 0;
  col_offset[1] = col_offset[1-1] + (&row.x2 - &row.x1);
  col_offset[2] = col_offset[2-1] + (&row.x3 - &row.x2);
  col_offset[3] = col_offset[3-1] + (&row.x4 - &row.x3);
  col_offset[4] = col_offset[4-1] + (&row.x5 - &row.x4);
  col_offset[5] = col_offset[5-1] + (&row.x6 - &row.x5);

  cols = malloc(n_cols * sizeof(char *));
  cols[0] = colI1; 
  cols[1] = colI3; 
  cols[2] = colI4; 
  cols[3] = colI8; 
  cols[4] = colF4; 
  cols[5] = colF8; 

  for ( int i = 0; i < n_rows; i++ ) {
    colI1[i] = 1*(i+1); 
    colI2[i] = 10*(i+1); 
    colI4[i] = 100*(i+1); 
    colI8[i] = 1000*(i+1); 
    colF4[i] = 10000*(i+1); 
    colF8[i] = 100000*(i+1); 
  }
  // STOP: set up input
  // START: Allocate output 
  out = malloc(n_rows * sizeof(my_row_t));
  memset(out, 0, n_rows * sizeof(my_row_t));
  //  STOP  Allocate output 
  status = transpose(cols, col_width, col_offset, n_cols, n_rows, out);
  for ( uint32_t i = 0; i < n_rows; i++ ) {
    pr_row(i, out+i);
  }
BYE:
  free_if_non_null(out);
  free_if_non_null(col_width);
  free_if_non_null(col_offset);
  free_if_non_null(cols);
  free_if_non_null(colI1);
  free_if_non_null(colI2);
  free_if_non_null(colI4);
  free_if_non_null(colI8);
  free_if_non_null(colF4);
  free_if_non_null(colF8);
}
