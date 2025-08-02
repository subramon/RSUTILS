#!/bin/bash
set -e
export RSUTILS_SRC_ROOT=${HOME}/RSUTILS/
export LD_LIBRARY_PATH=${RSUTILS_SRC_ROOT}/src/
gcc -g $QCFLAGS test_split_str.c ${RSUTILS_SRC_ROOT}/src/librsutils.so -std=gnu99 -I../../inc/ 
valgrind ./a.out "/tmp/" 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
