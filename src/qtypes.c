#include "q_incs.h"
#include "q_macros.h"
#include "cat_to_buf.h"
#include "qtypes.h"

qtype_t
get_tm_qtype(
    const char * const fld
    )
{
  if ( strcmp(fld, "tm_year") == 0 ) { return I1; }
  if ( strcmp(fld, "tm_mon")  == 0 ) { return I1; }
  if ( strcmp(fld, "tm_mday") == 0 ) { return I1; }
  if ( strcmp(fld, "tm_hour") == 0 ) { return I1; }
  if ( strcmp(fld, "tm_min")  == 0 ) { return I1; }
  if ( strcmp(fld, "tm_sec")  == 0 ) { return I1; }
  if ( strcmp(fld, "tm_yday") == 0 ) { return I2; }
  fprintf(stderr, "Bad tm fld = %s \n", fld);
  return 0; 
}

int
t_assign(
    struct tm *dst, 
    tm_t *src
    )
{
  int status = 0;
  if ( dst == NULL ) { go_BYE(-1); }
  if ( src == NULL ) {  go_BYE(-1); }
  memset(dst, 0, sizeof(struct tm));
  // dst->tm_sec  = src->tm_sec;
  // dst->tm_min  = src->tm_min;
  dst->tm_hour = src->tm_hour;
  dst->tm_mday = src->tm_mday;
  dst->tm_mon  = src->tm_mon;
  dst->tm_year = src->tm_year;
  dst->tm_wday = src->tm_wday;
  dst->tm_yday = src->tm_yday;
BYE:
  return status;
}

uint32_t
get_width_qtype(
    const char * const str_qtype
    )
{
  if ( str_qtype == NULL ) { WHEREAMI; return -1; }
  if ( strncmp(str_qtype, "SC:", strlen("SC:")) == 0 ) { 
    const char *cptr = str_qtype + strlen("SC:");
    for ( const char *xptr = cptr; *xptr != '\0'; xptr++ ) { 
      if ( !isdigit(*xptr) ) { WHEREAMI; return 0; }
    }
    int width = atoi(cptr);
    if ( width < 2 ) { WHEREAMI; return 0; } // need 1 for nullc
    if ( width > 16384 ) { WHEREAMI; return 0; } // some reasonable limit 
    return width;
  }
  qtype_t qtype = get_c_qtype(str_qtype);
  int width = get_width_c_qtype(qtype);
  return width;
}
// return string should be freed by caller 
char *
get_format(
    const char * const str_qtype
    )
{
  if ( str_qtype == NULL ) { WHEREAMI; return NULL; }
  if ( strncmp(str_qtype, "TM1:", strlen("TM1:")) == 0 ) { 
    const char *cptr = str_qtype + strlen("TM1:");
    if ( strlen(cptr) == 0 ) { WHEREAMI; return NULL; }
    return strdup(cptr);
  }
  if ( strncmp(str_qtype, "TM:", strlen("TM:")) == 0 ) { 
    const char *cptr = str_qtype + strlen("TM:");
    if ( strlen(cptr) == 0 ) { WHEREAMI; return NULL; }
    return strdup(cptr);
  }
  return NULL;
}

uint32_t
get_width_c_qtype(
      qtype_t qtype
    )
{
  switch ( qtype ) { 
    case B1 : return 0; break; // this is a special case, ugly
    case BL : return sizeof(bool); break;

    case I1 : return sizeof(int8_t); break;
    case I2 : return sizeof(int16_t); break;
    case I4 : return sizeof(int32_t); break;
    case I8 : return sizeof(int64_t); break;

    case UI1 : return sizeof(uint8_t); break;
    case UI2 : return sizeof(uint16_t); break;
    case UI4 : return sizeof(uint32_t); break;
    case UI8 : return sizeof(uint64_t); break;

    case F2 : return sizeof(bfloat16); break;
    case F4 : return sizeof(float); break;
    case F8 : return sizeof(double); break;

    case TM1 : return sizeof(tm_t); break;
    case TM  : return sizeof(struct tm); break;

    default : return 0; break;
  }
}

bool
is_qtype(
    const char *const str_qtype
    )
{
  if ( get_c_qtype(str_qtype) == Q0 ) {
    return false;
  }
  else {
    return true;
  }
}

qtype_t
get_c_qtype(
    const char *const str_qtype
    )
{
  if ( str_qtype == NULL ) { return Q0; }

  if ( strcmp("B1", str_qtype) == 0 ) { return B1; }
  if ( strcmp("BL", str_qtype) == 0 ) { return BL; }

  if ( strcmp("I1", str_qtype) == 0 ) { return I1; }
  if ( strcmp("I2", str_qtype) == 0 ) { return I2; }
  if ( strcmp("I4", str_qtype) == 0 ) { return I4; }
  if ( strcmp("I8", str_qtype) == 0 ) { return I8; }

  if ( strcmp("UI1", str_qtype) == 0 ) { return UI1; }
  if ( strcmp("UI2", str_qtype) == 0 ) { return UI2; }
  if ( strcmp("UI4", str_qtype) == 0 ) { return UI4; }
  if ( strcmp("UI8", str_qtype) == 0 ) { return UI8; }

  if ( strcmp("F2", str_qtype) == 0 ) { return F2; }
  if ( strcmp("F4", str_qtype) == 0 ) { return F4; }
  if ( strcmp("F8", str_qtype) == 0 ) { return F8; }

  if ( strcmp("SC", str_qtype) == 0 ) { return   SC; }  
  if ( strncmp("SC:", str_qtype, 3) == 0 ) { return SC; }   // NOTE

  if ( strcmp("TM", str_qtype) == 0 ) { return TM; }  
  if ( strncmp("TM:", str_qtype, 3) == 0 ) { return TM; }  

  if ( strcmp("TM1", str_qtype) == 0 ) { return TM1; }  
  if ( strncmp("TM1:", str_qtype, 4) == 0 ) { return TM1; }  

  return Q0;
}
const char *
get_str_qtype(
    qtype_t qtype
    )
{
  if ( qtype == Q0 ) { return "Q0"; }

  if ( qtype == B1 ) { return "B1"; }
  if ( qtype == BL ) { return "BL"; }

  if ( qtype == I1 ) { return "I1"; }
  if ( qtype == I2 ) { return "I2"; }
  if ( qtype == I4 ) { return "I4"; }
  if ( qtype == I8 ) { return "I8"; }
  if ( qtype == I16 ) { return "I16"; }

  if ( qtype == UI1 ) { return "UI1"; }
  if ( qtype == UI2 ) { return "UI2"; }
  if ( qtype == UI4 ) { return "UI4"; }
  if ( qtype == UI8 ) { return "UI8"; }
  if ( qtype == UI16 ) { return "UI16"; }

  if ( qtype == F2 ) { return "F2"; }
  if ( qtype == F4 ) { return "F4"; }
  if ( qtype == F8 ) { return "F8"; }

  if ( qtype == SC ) { return "SC"; }
  if ( qtype == SV ) { return "SV"; }
  if ( qtype == TM ) { return "TM"; }
  if ( qtype == TM1 ) { return "TM1"; }


  return NULL; 
}

const char *
str_qtype_to_str_ctype(
    const char * const str_qtype
    )
{
  if ( strcmp(str_qtype, "Q0") == 0 ) { return NULL; } 

  if ( strcmp(str_qtype, "B1") == 0 ) { return "uint64_t"; } 
  if ( strcmp(str_qtype, "BL") == 0 ) { return "bool"; } 

  if ( strcmp(str_qtype, "I1") == 0 ) { return "int8_t"; } 
  if ( strcmp(str_qtype, "I2") == 0 ) { return "int16_t"; } 
  if ( strcmp(str_qtype, "I4") == 0 ) { return "int32_t"; } 
  if ( strcmp(str_qtype, "I8") == 0 ) { return "int64_t"; } 

  if ( strcmp(str_qtype, "UI1") == 0 ) { return "uint8_t"; } 
  if ( strcmp(str_qtype, "UI2") == 0 ) { return "uint16_t"; } 
  if ( strcmp(str_qtype, "UI4") == 0 ) { return "uint32_t"; } 
  if ( strcmp(str_qtype, "UI8") == 0 ) { return "uint64_t"; } 

  if ( strcmp(str_qtype, "F2") == 0 ) { return "bfloat16"; } 
  if ( strcmp(str_qtype, "F4") == 0 ) { return "float"; } 
  if ( strcmp(str_qtype, "F8") == 0 ) { return "double"; } 

  if ( strcmp(str_qtype, "TM") == 0 ) { return "struct tm"; } 
  if ( strcmp(str_qtype, "TM1") == 0 ) { return "tm_t"; } 

  return NULL; 
}

// NOTE: Not all types supported for ispc
const char *
str_qtype_to_str_ispctype(
    const char * const str_qtype
    )
{
  if ( strcmp(str_qtype, "Q0") == 0 ) { return NULL; } 

  if ( strcmp(str_qtype, "B1") == 0 ) { return NULL; } 
  if ( strcmp(str_qtype, "BL") == 0 ) { return "int8"; } 

  if ( strcmp(str_qtype, "I1") == 0 ) { return "int8"; } 
  if ( strcmp(str_qtype, "I2") == 0 ) { return "int16"; } 
  if ( strcmp(str_qtype, "I4") == 0 ) { return "int32"; } 
  if ( strcmp(str_qtype, "I8") == 0 ) { return "int64"; } 

  if ( strcmp(str_qtype, "UI1") == 0 ) { return "uint8"; } 
  if ( strcmp(str_qtype, "UI2") == 0 ) { return "uint16"; } 
  if ( strcmp(str_qtype, "UI4") == 0 ) { return "uint32"; } 
  if ( strcmp(str_qtype, "UI8") == 0 ) { return "uint64"; } 

  if ( strcmp(str_qtype, "F2") == 0 ) { return NULL; }
  if ( strcmp(str_qtype, "F4") == 0 ) { return "float"; } 
  if ( strcmp(str_qtype, "F8") == 0 ) { return "double"; } 

  if ( strcmp(str_qtype, "TM") == 0 ) { return NULL; } 
  if ( strcmp(str_qtype, "TM1") == 0 ) { return NULL; } 
  if ( strcmp(str_qtype, "CUSTOM1") == 0 ) { return NULL; } 
  return NULL; 
}

bfloat16
F4_to_F2(
    float x
    )
{
  bfloat16 y = 0;
  memcpy(&y, ((char *)&x)+2, 2);

  return y;
}

float
F2_to_F4(
    bfloat16 x
    )
{
  float y = 0;
  memcpy(((char *)&y)+2, &x, 2);

  return y;
}

char *
qtypes_as_lua_tbl(
    void
    )
{
  int status = 0;
  char *out_str  = NULL;  uint32_t out_sz = 0, out_len = 0;
  char buf[64]; memset(buf, 0, 64);
  sprintf(buf, "local qtypes = {}\n");
  status = cat_to_buf(&out_str, &out_sz, &out_len, buf, 0); cBYE(status);

  int idx = 0;
  for ( qtype_t qt = Q0; qt < QF; qt++ ) { 
    const char *str = get_str_qtype(qt); if ( str == NULL ) { go_BYE(-1); }
    sprintf(buf, "qtypes['%s'] = %d \n", get_str_qtype(qt), idx++);
    status = cat_to_buf(&out_str, &out_sz, &out_len, buf, 0); cBYE(status);
  }
  sprintf(buf, "return qtypes\n");
  status = cat_to_buf(&out_str, &out_sz, &out_len, buf, 0); cBYE(status);
BYE:
  if ( status != 0 ) { return NULL; } else { return out_str; }
}
