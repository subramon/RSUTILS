#!/bin/ash
set -e
export PATH=$PATH:${RSUTILS_SRC_ROOT}/src/
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${RSUTILS_SRC_ROOT}/src/"
gcc test_run_cmd_capture.c ../../src/run_cmd_capture.c \
  -I../../inc/ \
  -o test_run_cmd_capture
valgrind ./test_run_cmd_capture 
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
rm -f _*
