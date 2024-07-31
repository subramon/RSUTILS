#include "q_incs.h"
#include "qtypes.h"
#include "sort1.h"
#include "unique.h"

static int
copy_where_good(
    const char * const in_vals, 
    const char *const nn, 
    uint32_t in_n, 
    qtype_t qtype,
    char * restrict vals, 
    int n
    )
{
  int status = 0;
  int outidx = 0;
  switch ( qtype ) { 
    case I1 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((int8_t *)vals)[outidx++] = ((const int8_t *const)in_vals)[i];
        }
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((int16_t *)vals)[outidx++] = ((const int16_t *const)in_vals)[i];
        }
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((int32_t *)vals)[outidx++] = ((const int32_t *const)in_vals)[i];
        }
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((int64_t *)vals)[outidx++] = ((const int64_t *const)in_vals)[i];
        }
      }
      break;
    case F2 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((bfloat16 *)vals)[outidx++] = ((const bfloat16 *const)in_vals)[i];
        }
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((float *)vals)[outidx++] = ((const float *const)in_vals)[i];
        }
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < in_n; i++ ) {
        if ( nn[i] == 1 ) {
          ((double *)vals)[outidx++] = ((const double *const)in_vals)[i];
        }
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
  if ( outidx != n ) { go_BYE(-1); }
BYE:
  return status;
}

// TODO P4: This implementation is slow since it uses a sort
// to compute number of uniques. Can be improved. 
// TODO P4: This code should be auto-generated

int 
num_unique(
    const char * const in_vals,  // [n] 
    const char *const nn, // [n]  tells us whether value is null or not. 0=> null
    uint32_t in_n, 
    qtype_t qtype, 
    uint32_t * restrict ptr_num
    )
{
  int status = 0;
  void *vals = NULL;  uint32_t n = 0;
  if ( in_vals == NULL ) { go_BYE(-1); }
  if ( in_n   <= 0     ) { go_BYE(-1); }
  uint32_t num = 1;
  int8_t   valI1; const int8_t   *I1vals = (const int8_t * const)in_vals;
  int16_t  valI2; const int16_t  *I2vals = (const int16_t * const)in_vals;
  int32_t  valI4; const int32_t  *I4vals = (const int32_t * const)in_vals;
  int64_t  valI8; const int64_t  *I8vals = (const int64_t * const)in_vals;
  bfloat16 valF2; const bfloat16 *F2vals = (const bfloat16 * const)in_vals;
  float    valF4; const float    *F4vals = (const float * const)in_vals;
  double   valF8; const double   *F8vals = (const double * const)in_vals;
  int width = get_width_c_qtype(qtype); if ( width <= 0 ) { go_BYE(-1); }

  vals = malloc(in_n * width); return_if_malloc_failed(vals);
  if ( nn == NULL ) { 
    memcpy(vals, in_vals, in_n*width); n = in_n;
  }
  else {
    for ( uint32_t i = 0; i < in_n; i++ ) { 
      if ( ( nn[i] != 0 ) && ( nn[i] != 1 ) ) { go_BYE(-1); }
      n += nn[i];
    }
    if ( n == 0 ) { *ptr_num = 0; goto BYE; } // all null values
    status = copy_where_good(in_vals, nn, in_n, qtype, vals, n); 
    cBYE(status);
  }

  switch ( qtype ) { 
    case I1 : 
      I1vals = (int8_t *)vals;
      qsort(vals, n, width, fn_sortI1_asc);
      valI1 = I1vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( I1vals[i] != valI1 ) { 
          num++;
          valI1 = I1vals[i];
        }
      }
      break;
    case I2 : 
      I2vals = (int16_t *)vals;
      qsort(vals, n, width, fn_sortI2_asc);
      valI2 = I2vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( I2vals[i] != valI2 ) { 
          num++;
          valI2 = I2vals[i];
        }
      }
      break;
    case I4 : 
      I4vals = (int32_t *)vals;
      qsort(vals, n, width, fn_sortI4_asc);
      valI4 = I4vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( I4vals[i] != valI4 ) { 
          num++;
          valI4 = I4vals[i];
        }
      }
      break;
    case I8 : 
      I8vals = (int64_t *)vals;
      qsort(vals, n, width, fn_sortI8_asc);
      valI8 = I8vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( I8vals[i] != valI8 ) { 
          num++;
          valI8 = I8vals[i];
        }
      }
      break;
    case F2 : 
      F2vals = (bfloat16 *)vals;
      qsort(vals, n, width, fn_sortF2_asc);
      valF2 = F2vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( F2vals[i] != valF2 ) { 
          num++;
          valF2 = F2vals[i];
        }
      }
      break;
    case F4 : 
      F4vals = (float *)vals;
      qsort(vals, n, width, fn_sortF4_asc);
      valF4 = F4vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( F4vals[i] != valF4 ) { 
          num++;
          valF4 = F4vals[i];
        }
      }
      break;
    case F8 : 
      F8vals = (double *)vals;
      qsort(vals, n, width, fn_sortF8_asc);
      valF8 = F8vals[0];
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( F8vals[i] != valF8 ) { 
          num++;
          valF8 = F8vals[i];
        }
      }
      break;
    default : 
      fprintf(stderr, "TODO P4\n"); go_BYE(-1);
      break;
  }
  *ptr_num = num;
BYE:
  free_if_non_null(vals); 
  return status;
}

// all null values => is_uniuue = false
bool
is_unique(
    const char * const vals,  // [n] 
    const char * const nn, // [n]  nn[i] = 0 > ith element is null 
    uint32_t n, 
    qtype_t qtype
    )
{
  if ( vals == NULL ) { WHEREAMI; return false; }
  if ( n   <= 0     ) { WHEREAMI; return false; }

  int8_t  this_valI1, valI1;
  int16_t this_valI2, valI2;
  int32_t this_valI4, valI4;
  int64_t this_valI8, valI8;
  float   this_valF4, valF4;
  double  this_valF8, valF8;

  bool val_seen = false;
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( ( nn == NULL ) || ( nn[i] == 1 ) ) {
      switch ( qtype ) { 
        case I1 : 
          this_valI1 = ((const int8_t *const)vals)[i];
          if ( val_seen == false ) { 
            valI1 = ((const int8_t *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valI1 == this_valI1 ) { return false; }
          }
          break;
        case I2 : 
          this_valI2 = ((const int16_t *const)vals)[i];
          if ( val_seen == false ) { 
            valI2 = ((const int16_t *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valI2 == this_valI2 ) { return false; }
          }
          break;
        case I4 : 
          this_valI4 = ((const int32_t *const)vals)[i];
          if ( val_seen == false ) { 
            valI4 = ((const int32_t *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valI4 == this_valI4 ) { 
              return false; }
          }
          break;
        case I8 : 
          this_valI8 = ((const int64_t *const)vals)[i];
          if ( val_seen == false ) { 
            valI8 = ((const int64_t *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valI8 == this_valI8 ) { return false; }
          }
          break;
        case F4 : 
          this_valF4 = ((const float *const)vals)[i];
          if ( val_seen == false ) { 
            valF4 = ((const float *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valF4 == this_valF4 ) { return false; } }
          break;
        case F8 : 
          this_valF8 = ((const double *const)vals)[i];
          if ( val_seen == false ) { 
            valF8 = ((const double *const)vals)[i];
            val_seen = true; 
          }
          else {
            if ( valF8 == this_valF8 ) { return false; }
          }
          break;
        default : 
          WHEREAMI; return false;
          break;
      }
    }
  }
  if ( val_seen == false ) { return false; }
  return true;
}
 
int 
num_unique_saturated(
    const char * const vals,  // [n] 
    uint32_t n, 
    qtype_t qtype,
    uint32_t max_n_uq,
    uint32_t * restrict ptr_n_uq
    )
{
  int status = 0;
  void *uq_vals = NULL; // [n_uq]
  uint32_t n_uq = 0; 
  uint32_t max_w = sizeof(double); // TODO P4 can handle only limited qtypes

  if ( vals == NULL ) { go_BYE(-1); }
  if ( n   <= 0     ) { go_BYE(-1); }
  if ( max_n_uq <= 0 ) { go_BYE(-1); }
  uq_vals = malloc(max_n_uq * max_w);
  return_if_malloc_failed(uq_vals);

  switch ( qtype ) { 
    case I1 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((int8_t *)uq_vals)[j] == ((const int8_t *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((int8_t *)uq_vals)[n_uq++] = ((const int8_t *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    case I2 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((int16_t *)uq_vals)[j] == ((const int16_t *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((int16_t *)uq_vals)[n_uq++] = ((const int16_t *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    case I4 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((int32_t *)uq_vals)[j] == ((const int32_t *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((int32_t *)uq_vals)[n_uq++] = ((const int32_t *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    case I8 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((int64_t *)uq_vals)[j] == ((const int64_t *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((int64_t *)uq_vals)[n_uq++] = ((const int64_t *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    case F4 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((float *)uq_vals)[j] == ((const float *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((float *)uq_vals)[n_uq++] = ((const float *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    case F8 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        bool found = false;
        for ( uint32_t j = 0; j < n_uq; j++ ) { 
          if ( ((double *)uq_vals)[j] == ((const double *const)vals)[i] ) { 
            found = true; break;
          }
        }
        if ( !found ) { 
          if ( n_uq >= max_n_uq ) { goto BYE; } 
          ((double *)uq_vals)[n_uq++] = ((const double *const)vals)[i];
        }
      }
      break;
      //-----------------------------------------
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  free_if_non_null(uq_vals);
  *ptr_n_uq = n_uq;
  return status;
}
