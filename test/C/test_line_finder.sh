#!/bin/bash
set -e
test -d ${RSUTILS_SRC_ROOT}
gcc test_line_finder.c ../../src/line_finder.c \
  -g -std=gnu99 \
  -I../../inc/ -lm
valgrind ./a.out 1>_out 2>_x
grep  "0 errors from 0 contexts" _x 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
