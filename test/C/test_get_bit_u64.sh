#!/bin/bash
set -e 
gcc  $QC_FLAGS -g \
  test_get_bit_u64.c \
  ../../src/get_bit_u64.c  \
  ../../src/rdtsc.c  \
  ../../src/get_time_usec.c  \
  -I../../inc/ 
valgrind ./a.out $filename 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
