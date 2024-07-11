#!/bin/bash
set -e
test -d ${RSUTILS_SRC_ROOT}
filename=${RSUTILS_SRC_ROOT}/test/C/test_file_exists.c
test -f $filename
gcc -g test_file_exists.c ../../src/file_exists.c \
  -g -std=gnu99 \
  -I../../inc/ -lm
valgrind ./a.out $filename 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
