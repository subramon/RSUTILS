#!/bin/bash
set -e 
gcc  -g test_str_between.c ../../src/str_between.c -std=gnu99 -I../../inc/ -I../../gen_inc/ 
valgrind ./a.out 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
