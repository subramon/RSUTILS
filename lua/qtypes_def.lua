 return [[ 

typedef uint16_t bfloat16;
typedef enum {
  Q0,

  B1,
  BL,

  I1,
  I2,
  I4,
  I8,
  I16,

  F2,
  F4,
  F8,

  UI1,
  UI2,
  UI4,
  UI8,
  UI16,

  SC,
  SV,
  TM,
  TM1,

  QF
} qtype_t;
typedef struct _tm_t {
  int16_t tm_year;
  int8_t tm_mon;
  int8_t tm_mday;
  int8_t tm_hour;


  int8_t tm_wday;
  int16_t tm_yday;
} tm_t;

 ]] 
