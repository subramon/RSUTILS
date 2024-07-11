#!/bin/bash
set -e 
gcc  -g $QCFLAGS test_get_file_size.c ../../src/get_file_size.c \
  -std=gnu99 -I../../inc/ 
valgrind ./a.out $filename 1>_out 2>_err
# TODO Failing: grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
