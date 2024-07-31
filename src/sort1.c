#include "q_incs.h"
#include "qtypes.h"
#include "bf16tofp32.h"
#include "sort1.h"

int
fn_sortI1_asc(const void *p1, const void *p2)
{
  const int8_t  *u1 = (const int8_t *)p1;
  const int8_t  *u2 = (const int8_t *)p2;
  int8_t v1 = *u1;
  int8_t v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortI2_asc(const void *p1, const void *p2)
{
  const int16_t  *u1 = (const int16_t *)p1;
  const int16_t  *u2 = (const int16_t *)p2;
  int16_t v1 = *u1;
  int16_t v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortI4_asc(const void *p1, const void *p2)
{
  const int32_t  *u1 = (const int32_t *)p1;
  const int32_t  *u2 = (const int32_t *)p2;
  int32_t v1 = *u1;
  int32_t v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortI8_asc(const void *p1, const void *p2)
{
  const int64_t  *u1 = (const int64_t *)p1;
  const int64_t  *u2 = (const int64_t *)p2;
  int64_t v1 = *u1;
  int64_t v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortI8_dsc(const void *p1, const void *p2)
{
  const int64_t  *u1 = (const int64_t *)p1;
  const int64_t  *u2 = (const int64_t *)p2;
  int64_t v1 = *u1;
  int64_t v2 = *u2;
  if ( v1 > v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortF2_asc(const void *p1, const void *p2)
{
  const bfloat16  *u1 = (const bfloat16 *)p1;
  const bfloat16  *u2 = (const bfloat16 *)p2;
  float v1 = bf16tofp32(*u1);
  float v2 = bf16tofp32(*u2);
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortF2_dsc(const void *p1, const void *p2)
{
  const bfloat16  *u1 = (const bfloat16 *)p1;
  const bfloat16  *u2 = (const bfloat16 *)p2;
  float v1 = bf16tofp32(*u1);
  float v2 = bf16tofp32(*u2);
  if ( v1 > v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortF4_asc(const void *p1, const void *p2)
{
  const float  *u1 = (const float *)p1;
  const float  *u2 = (const float *)p2;
  float v1 = *u1;
  float v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortF4_dsc(const void *p1, const void *p2)
{
  const float  *u1 = (const float *)p1;
  const float  *u2 = (const float *)p2;
  float v1 = *u1;
  float v2 = *u2;
  if ( v1 > v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
int
fn_sortF8_asc(const void *p1, const void *p2)
{
  const double  *u1 = (const double *)p1;
  const double  *u2 = (const double *)p2;
  double v1 = *u1;
  double v2 = *u2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}
