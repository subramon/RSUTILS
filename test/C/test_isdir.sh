#!/bin/bash
set -e
gcc -g $QCFLAGS test_isdir.c ../../src/isdir.c \
  -std=gnu99 -I../../inc/ 
valgrind ./a.out "/tmp/" 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
