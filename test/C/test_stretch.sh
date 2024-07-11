#!/bin/bash
set -e 
gcc  -g $QCFLAGS test_stretch.c ../../src/stretch.c \
  ../../src/rand_file_name.c ../../src/get_file_size.c \
  ../../src/isfile.c \
  -std=gnu99 -I../../inc/ 
valgrind ./a.out $filename 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
