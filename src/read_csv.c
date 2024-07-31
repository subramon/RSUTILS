#include "q_incs.h"
#include "qtypes.h"
#include "rs_mmap.h"
#include "read_csv.h"
#include <time.h>
// TODO P4 Why do as an include not obviate need for below
extern char *strptime(const char *s, const char *format, struct tm *tm);
// TODO Consider converting [const char ** const qtypes], to [int *c_qtypes]
// for performance 
int
read_csv(
    const char * const infile,
    const char ** const str_qtypes,
    void ** const out, // [ncols][nrows]
    const uint32_t * const widths, // [ncols] width of a column (needed for SC)
    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
#define BUFSZ 511 // Good enough for largest cell
  char *buf = NULL;
  if ( infile == NULL ) { go_BYE(-1); }
  if ( out    == NULL ) { go_BYE(-1); }

  if ( str_fld_sep == NULL ) { go_BYE(-1); }
  if ( str_fld_delim == NULL ) { go_BYE(-1); }
  if ( str_rec_sep == NULL ) { go_BYE(-1); }

  if ( strlen(str_fld_sep) != 1 ) { go_BYE(-1); }
  if ( strlen(str_fld_delim) != 1 ) { go_BYE(-1); }
  if ( strlen(str_rec_sep) != 1 ) { go_BYE(-1); }

  char fld_sep   = str_fld_sep[0];
  char fld_delim = str_fld_delim[0];
  char rec_sep   = str_rec_sep[0];
  status = rs_mmap(infile, &X, &nX, 0); cBYE(status);

  size_t xidx = 0;
  if ( is_hdr ) {  // skip over first line 
    for ( ; xidx < nX; xidx++ ) { 
      if ( X[xidx] == rec_sep ) { 
        xidx++;
        break;
      }
    }
  }
  // allocate buffer for a single cell value
  buf = malloc(BUFSZ+1);
  return_if_malloc_failed(buf);
  for ( uint32_t i = 0; i < nrows; i++ ) {
    for ( uint32_t j = 0; j < ncols; j++ ) { 
      if ( xidx >= nX ) { 
        go_BYE(-1); }
      char c = X[xidx];
      memset(buf, 0, BUFSZ+1); //  TODO P3 Delete once tested
      char terminator; 
      if ( j == (ncols-1) ) { 
        terminator = rec_sep;
      }
      else {
        terminator = fld_sep;
      }
      if ( c == fld_delim ) { 
        for ( uint32_t k = 0; k < BUFSZ; k++ ) { 
          xidx++; 
          if ( xidx >= nX ) { go_BYE(-1); }
          c = X[xidx];
          if ( c == fld_delim ) {
            xidx++;
            if ( xidx >= nX ) { go_BYE(-1); }
            c = X[xidx];
            if ( c != terminator ) { 
              go_BYE(-1); }
            xidx++;
            break;
          }
          buf[k] = c;
        }
      }
      else {
        buf[0] = c;
        for ( uint32_t k = 1; k < BUFSZ; k++ ) { 
          xidx++;
          if ( xidx >= nX ) { go_BYE(-1); }
          c = X[xidx];
          if ( c == terminator ) { xidx++; break; }
          buf[k] = c;
        }
      }
      // now  we have a single cell value
      char *endptr;
      //-------------------------------------------
      if ( strcmp(str_qtypes[j], "F8") == 0 ) { 
        double dval = strtod(buf, &endptr);
        double *dptr = (double *)out[j];
        dptr[i] = dval;
      }
      else if ( strcmp(str_qtypes[j], "F4") == 0 ) { 
        double dval = strtod(buf, &endptr);
        if ( ( dval > FLT_MAX ) || ( dval < -FLT_MAX ) ) { go_BYE(-1); }
        float *fptr = (float *)out[j];
        fptr[i] = dval;
      }
      //-------------------------------------------
      else if ( strcmp(str_qtypes[j], "F2") == 0 ) { 
        double dval = strtod(buf, &endptr);
        if ( ( dval > FLT_MAX ) || ( dval < -FLT_MAX ) ) { go_BYE(-1); }
        bfloat16 *fptr = (bfloat16 *)out[j];
        fptr[i] = dval;
      }
      else if ( strcmp(str_qtypes[j], "I8") == 0 ) { 
        long long int ival = strtoll(buf, &endptr, 10);
        int64_t *iptr = (int64_t *)out[j];
        iptr[i] = ival;
      }
      else if ( strcmp(str_qtypes[j], "I4") == 0 ) { 
        long int ival = strtol(buf, &endptr, 10);
        if ( ( ival > INT_MAX ) || ( ival < INT_MIN ) ) { go_BYE(-1); }
        int32_t *iptr = (int32_t *)out[j];
        iptr[i] = ival;
      }
      else if ( strcmp(str_qtypes[j], "I1") == 0 ) { 
        long int ival = strtol(buf, &endptr, 10);
        if ( ( ival > SCHAR_MAX ) || ( ival < SCHAR_MIN ) ) { go_BYE(-1); }
        int8_t *iptr = (int8_t *)out[j];
        iptr[i] = ival;
      }
      else if ( strcmp(str_qtypes[j], "I2") == 0 ) { 
        long int ival = strtol(buf, &endptr, 10);
        if ( ( ival > SHRT_MAX ) || ( ival < SHRT_MIN ) ) { go_BYE(-1); }
        int16_t *iptr = (int16_t *)out[j];
        iptr[i] = ival;
      }
      //-------------------------------------------
      else if ( strcmp(str_qtypes[j], "UI8") == 0 ) { 
        unsigned long long int uival = strtoull(buf, &endptr, 10);
        uint64_t *uiptr = (uint64_t *)out[j];
        uiptr[i] = uival;
      }
      else if ( strcmp(str_qtypes[j], "UI4") == 0 ) { 
        unsigned long int uival = strtol(buf, &endptr, 10);
        if ( uival > UINT_MAX ) { go_BYE(-1); }
        uint32_t *uiptr = (uint32_t *)out[j];
        uiptr[i] = uival;
      }
      else if ( strcmp(str_qtypes[j], "UI2") == 0 ) { 
        unsigned long long int uival = strtoull(buf, &endptr, 10);
        if ( uival > USHRT_MAX ) { go_BYE(-1); }
        uint16_t *uiptr = (uint16_t *)out[j];
        uiptr[i] = uival;
      }
      else if ( strcmp(str_qtypes[j], "UI1") == 0 ) { 
        unsigned long long int uival = strtoull(buf, &endptr, 10);
        if ( uival > UCHAR_MAX ) { go_BYE(-1); }
        uint8_t *uiptr = (uint8_t *)out[j];
        uiptr[i] = uival;
      }
      //-------------------------------------------
      else if ( strncmp(str_qtypes[j], "TM:", 3) == 0 ) {
        const char * const format = str_qtypes[j] + 3;
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
      else if ( strncmp(str_qtypes[j], "SC:", 3) == 0 ) {
        if ( strncmp(str_qtypes[j], "SC:", strlen("SC:")) == 0 ) { 
          if ( widths == NULL ) { go_BYE(-1); } 
          int width = widths[j];
          if ( width < 1 ) { go_BYE(-1); } 
          if ( (uint32_t)width < strlen(buf)+1 ) { go_BYE(-1); } 
          char *cptr = out[j]; cptr += (i*width);
          memset(cptr, 0, width);
          memcpy(cptr, buf, strlen(buf));
        }
      }
      else if ( strcmp(str_qtypes[j], "XX") == 0 ) { 
        // do nothing, we are ignoring this column
      }
      else {
        go_BYE(-1);
      }
    }
  }

BYE:
  free_if_non_null(buf);
  if ( X != NULL ) { munmap(X, nX); }
  return status;
}
