#!/bin/ash
set -e
export PATH=$PATH:${RSUTILS_SRC_ROOT}/src/
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${RSUTILS_SRC_ROOT}/src/"
valgrind cli_asc_to_bin ./data/colF4.txt F4 _colF4.bin true 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
od -f colF4.bin
rm -f _*
