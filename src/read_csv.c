#include "q_incs.h"
#include "qtypes.h"
#include "rs_mmap.h"
#include <time.h>
// TODO P4 strptime should be in time.h. But following still needed
extern char *strptime(const char *s, const char *format, struct tm *tm);
#include "read_csv.h"

#define mcr_chk_endptr(x, y) { \
        if ( ( x != NULL ) && ( *x != '\0' ) && ( *x != '\n' ) ) { \
          fprintf(stderr, "Unable to convert cell [%s]\n", y); \
          go_BYE(-1); \
        } \
}
int
read_csv(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    const qtype_t * const qtypes, // [ncols] 
    const uint32_t * const widths, // [ncols] (needed for SC)
    char ** const formats, // [ncols] (needed for TM)
    const bool * const is_load, // [ncols] whether col has nulls
    const bool * const has_nulls, // [ncols] whether col to be loadde

    char ** const out, // [ncols][nrows]
    bool ** const nn_out, // [ncols][nrows]

    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    )
{
  int status = 0;
  char bslash = '\\';
  bool is_load_j, has_nulls_j;
  char *X = NULL; size_t nX = 0;
  int bufsz = 511;  // default 
  char *buf = NULL;
  if ( widths != NULL ) {
    for ( uint32_t i = 0; i < ncols; i++ ) { 
      if ( bufsz < 2*widths[i] ) { bufsz = 2*widths[i]; }
    }
  }


  // START: Check input arguments 
  // Provide either infile or in_X, in_nX but not both 
  if ( infile == NULL ) { 
    if ( ( in_X == NULL ) || ( in_nX == 0 ) ) { go_BYE(-1); }
  }
  else {
    if ( ( in_X != NULL ) || ( in_nX != 0 ) ) { go_BYE(-1); }
  }
  if ( out    == NULL ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < ncols; i++ ) { 
    bool is_load_i = true;
    if ( ( is_load != NULL ) && ( is_load[i] == false ) )  {
      is_load_i = false;
    }
    bool has_nulls_i = false;
    if ( ( has_nulls != NULL ) && ( has_nulls[i] == true ) )  {
      has_nulls_i = true;
    }
    if ( is_load_i && has_nulls_i ) {
      if ( ( nn_out == NULL ) || ( nn_out[i] == NULL ) ) { 
        go_BYE(-1);
      }
    }
  }


  if ( str_fld_sep == NULL ) { go_BYE(-1); }
  if ( str_fld_delim == NULL ) { go_BYE(-1); }
  if ( str_rec_sep == NULL ) { go_BYE(-1); }

  if ( strlen(str_fld_sep) != 1 ) { go_BYE(-1); }
  if ( strlen(str_fld_delim) != 1 ) { go_BYE(-1); }
  if ( strlen(str_rec_sep) != 1 ) { go_BYE(-1); }
  // STOP : Check input arguments 

  char fld_sep   = str_fld_sep[0];
  char fld_delim = str_fld_delim[0];
  char rec_sep   = str_rec_sep[0];
  if ( infile != NULL ) { 
    status = rs_mmap(infile, &X, &nX, 0); cBYE(status);
  }
  else {
    X = in_X;
    nX = in_nX;
  }

  size_t xidx = 0;
  if ( is_hdr ) {  // skip over first line 
    bool found_eoln = false;
    for ( uint64_t i = 0; i < nX; i++ ) { 
      if ( X[i] == '\n' ) { 
        X += i+1;
        nX -= i+1;
        found_eoln = true;
        break;
      }
    }
    if ( !found_eoln ){ go_BYE(-1); }
  }
  // allocate buffer for a single cell value
  buf = malloc(bufsz+1);
  return_if_malloc_failed(buf);
  memset(buf, 0, bufsz+1); 
  for ( uint32_t i = 0; i < nrows; i++ ) {
    for ( uint32_t j = 0; j < ncols; j++ ) {
      if ( xidx >= nX ) { go_BYE(-1); }
      char c = X[xidx];
      memset(buf, 0, bufsz+1); //  TODO P3 Delete once tested
      char terminator; 
      if ( j == (ncols-1) ) { 
        terminator = rec_sep;
      }
      else {
        terminator = fld_sep;
      }
      if ( c == terminator ) { 
        buf[0] = '\0';
        xidx++; 
        goto CELL_COMPLETE;
      }
      if ( c == fld_delim ) { 
        for ( uint32_t k = 0; k < bufsz; k++ ) { 
          xidx++; 
          if ( xidx >= nX ) { go_BYE(-1); }
          c = X[xidx];
          if ( c == bslash ) { 
            if ( xidx >= nX ) { go_BYE(-1); }
            xidx++;
            c = X[xidx];
            if ( ( c == bslash ) || ( c == fld_delim ) ) {
              // all is well
            }
            else {
              go_BYE(-1);
            }
          }
          else {
            if ( c == fld_delim ) {
              xidx++;
              if ( xidx >= nX ) { go_BYE(-1); }
              c = X[xidx];
              if ( c != terminator ) { 
                go_BYE(-1); }
              xidx++;
              break;
            }
          }
          buf[k] = c;
        }
      }
      else {
        buf[0] = c;
        for ( uint32_t k = 1; k < bufsz; k++ ) { 
          xidx++;
          if ( xidx >= nX ) { go_BYE(-1); }
          c = X[xidx];
          if ( c == terminator ) { xidx++; break; }
          buf[k] = c;
        }
      }
      char *endptr = NULL;
CELL_COMPLETE:
      // now  we have a single cell value
      //-------------------------------------------
      is_load_j = true;
      if ( ( is_load != NULL ) && ( is_load[j] == false ) )  {
        is_load_j= false;
      }
      if ( is_load_j == false ) { continue; }

      has_nulls_j = false;
      if ( ( has_nulls != NULL ) && ( has_nulls[j] == true ) )  {
        has_nulls_j = true;
        if ( nn_out[j] == NULL ) { go_BYE(-1); } 
      }
      else {
        if ( nn_out[j] != NULL ) { go_BYE(-1); } 
      }
      // determine if we have a null value with us 
      bool is_null;
      for ( ; ; ) {
        if ( strlen(buf) == 0 ) { is_null = true; break; }
        if ( ( strlen(buf) == 2 ) && 
            ( ( strncmp(buf, "\\N", 2) == 0 ) || 
              ( strncmp(buf, "NA", 2) == 0 ) ) ) { is_null = true; break; }
        is_null = false; break;
      }
      if ( is_null ) {
        if ( has_nulls_j == false ) { 
          printf("Got null value when none expected. ");
          printf(" for row %d column %d \n", i, j);
          go_BYE(-1);
        }
        else {
          nn_out[j][i] = false;
        }
        continue;
      }
      // Control comes here => value is not null 
      if ( nn_out[j] != NULL ) { 
        nn_out[j][i] = true;
      }
      switch ( qtypes[j] ) {
        case F8 : 
          {
            double dval = strtod(buf, &endptr);
            mcr_chk_endptr(endptr, buf);
            double *dptr = (double *)out[j];
            dptr[i] = dval;
          }
          break;
          //-------------------------------------------
        case F4 : 
          {
            double dval = strtod(buf, &endptr);
            mcr_chk_endptr(endptr, buf);
            if ( ( dval > FLT_MAX ) || ( dval < -FLT_MAX ) ) { go_BYE(-1); }
            float *fptr = (float *)out[j];
            fptr[i] = dval;
          }
          break;
          //-------------------------------------------
        case F2 : 
          {
            double dval = strtod(buf, &endptr);
            mcr_chk_endptr(endptr, buf);
            if ( ( dval > FLT_MAX ) || ( dval < -FLT_MAX ) ) { go_BYE(-1); }
            bfloat16 *fptr = (bfloat16 *)out[j];
            fptr[i] = dval;
          }
          break;
          //-------------------------------------------
        case I8 : 
          {
            long long int ival = strtoll(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            int64_t *iptr = (int64_t *)out[j];
            iptr[i] = ival;
          }
          break;
        case I4 : 
          {
            long int ival = strtol(buf, &endptr, 10);
            if ( ( endptr != NULL ) && ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { \
              printf("hello world\n");
            }

            mcr_chk_endptr(endptr, buf);
            if ( ( ival > INT_MAX ) || ( ival < INT_MIN ) ) { go_BYE(-1); }
            int32_t *iptr = (int32_t *)out[j];
            iptr[i] = ival;
          }
          break;
        case BL : 
          {
            bool *bptr = (bool *)out[j];
            if ( ( strcasecmp(buf, "true") == 0 )| ( strcasecmp(buf, "1") == 0 ) ) {
              bptr[i] = true;
            }
            else if ( ( strcasecmp(buf, "false") == 0 )| ( strcasecmp(buf, "0") == 0 ) ) {
              bptr[i] = false;
            }
            else { 
              fprintf(stderr, "Invalid value for BL = [%s] \n", buf);
              go_BYE(-1);
            }
          }
          break;
        case I1 : 
          {
            long int ival = strtol(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            if ( ( ival > SCHAR_MAX ) || ( ival < SCHAR_MIN ) ) { go_BYE(-1); }
            int8_t *iptr = (int8_t *)out[j];
            iptr[i] = ival;
          }
          break;
        case I2 : 
          {
            long int ival = strtol(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            if ( ( ival > SHRT_MAX ) || ( ival < SHRT_MIN ) ) { go_BYE(-1); }
            int16_t *iptr = (int16_t *)out[j];
            iptr[i] = ival;
          }
          break;
          //-------------------------------------------
        case UI8 : 
          {
            unsigned long long int uival = strtoull(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            uint64_t *uiptr = (uint64_t *)out[j];
            uiptr[i] = uival;
          }
          break;
        case UI4 : 
          {
            unsigned long int uival = strtol(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            if ( uival > UINT_MAX ) { go_BYE(-1); }
            uint32_t *uiptr = (uint32_t *)out[j];
            uiptr[i] = uival;
          }
          break; 
        case UI2 : 
          {
            unsigned long long int uival = strtoull(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            if ( uival > USHRT_MAX ) { go_BYE(-1); }
            uint16_t *uiptr = (uint16_t *)out[j];
            uiptr[i] = uival;
          }
          break;
        case UI1 : 
          {
            unsigned long long int uival = strtoull(buf, &endptr, 10);
            mcr_chk_endptr(endptr, buf);
            if ( uival > UCHAR_MAX ) { go_BYE(-1); }
            uint8_t *uiptr = (uint8_t *)out[j];
            uiptr[i] = uival;
          }
          break; 
          //-------------------------------------------
        case TM1 : 
          {
            // TODO: We have to deal with null values properly
            if ( *buf == '\0' ) { // null value 
              tm_t  *tptr = (tm_t *)out[j];
              memset(&(tptr[i]), 0, sizeof(tm_t));
            }
            else {
              if ( ( formats == NULL ) || ( formats[j] == NULL ) ) {
                go_BYE(-1);
              }
              const char * const format = formats[j];
              if ( *format == '\0' ) { go_BYE(-1); }
              tm_t  *tptr = (tm_t *)out[j];
              struct tm l_tm;
              memset(&l_tm, 0, sizeof(struct tm));
              char *rslt = strptime(buf, format, &l_tm);
              if ( rslt == NULL ) { 
                go_BYE(-1); }
              if ( l_tm.tm_year >= 255 ) { go_BYE(-1); } 
              if ( l_tm.tm_year <  0 ) { go_BYE(-1); } 
#ifdef DEBUG
              if ( ( l_tm.tm_sec < 0 ) || ( l_tm.tm_sec > 59 ) ) { go_BYE(-1); }
              if ( ( l_tm.tm_min < 0 ) || ( l_tm.tm_min > 59 ) ) { go_BYE(-1); }
              if ( ( l_tm.tm_hour < 0 ) || ( l_tm.tm_hour > 23 ) ) { go_BYE(-1); }
              if ( ( l_tm.tm_wday < 0 ) || ( l_tm.tm_wday > 6 ) ) { go_BYE(-1); }
              if ( ( l_tm.tm_yday < 0 ) || ( l_tm.tm_yday > 365 ) ) { go_BYE(-1); }
              if ( ( l_tm.tm_mon < 0 ) || ( l_tm.tm_mon > 12 ) ) { go_BYE(-1); }
#endif
              tptr[i].tm_year = l_tm.tm_year;
              tptr[i].tm_mon = l_tm.tm_mon;
              tptr[i].tm_mday = l_tm.tm_mday;
              tptr[i].tm_hour = l_tm.tm_hour;
              // tptr[i].tm_min = l_tm.tm_min;
              // tptr[i].tm_sec = l_tm.tm_sec;
              tptr[i].tm_wday = l_tm.tm_wday;
              tptr[i].tm_yday = l_tm.tm_yday;
            }
          }
          break;
        case SC : 
          {
            if ( widths == NULL ) { go_BYE(-1); } 
            uint32_t width = widths[j];
            if ( width < 1 ) { go_BYE(-1); } 
            if ( width < strlen(buf)+1 ) { 
              printf("Cell [%s] too large. Len <= %u\n", buf, width); 
              go_BYE(-1); } 
            char *cptr = out[j]; cptr += (i*width);
            memset(cptr, 0, width);
            memcpy(cptr, buf, strlen(buf));
          }
          break; 
        case Q0 :
          {
            go_BYE(-1);
            break;
          }
        default : 
          go_BYE(-1);
          break;
      }
      // fprintf(stdout, "(r=%6u:c=%2u:v=%s)\n", i, j,buf);
    }
  }

BYE:
  free_if_non_null(buf);
  if ( infile != NULL ) { if ( X != NULL ) { munmap(X, nX); } }
  return status;
}
