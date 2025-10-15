#!/bin/bash
set -e
export RSUTILS_SRC_ROOT=${HOME}/RSUTILS/
export LD_LIBRARY_PATH=${RSUTILS_SRC_ROOT}/src/
gcc -g -O0 -std=gnu99 $QCFLAGS -I${RSUTILS_SRC_ROOT}/inc/ \
  test_csp_parse.c  \
  ${RSUTILS_SRC_ROOT}/src/csp_parse.c \
  ${RSUTILS_SRC_ROOT}/src/csp_aux.c \
  ${RSUTILS_SRC_ROOT}/src/csp_synthesize.c \
  ${RSUTILS_SRC_ROOT}/src/free_2d_array.c \
  ${RSUTILS_SRC_ROOT}/src/cat_to_buf.c \
  ${RSUTILS_SRC_ROOT}/src/multiple.c \
  ${RSUTILS_SRC_ROOT}/src/bridge_csp.c \
  ${RSUTILS_SRC_ROOT}/src/file_as_str.c \
  ${RSUTILS_SRC_ROOT}/src/str_as_file.c \
  ${RSUTILS_SRC_ROOT}/src/str_between.c \
  ${RSUTILS_SRC_ROOT}/src/extract_name_value.c \
  ${RSUTILS_SRC_ROOT}/src/trim.c \
  ${RSUTILS_SRC_ROOT}/src/rs_mmap.c \
  -o test_csp_parse \
  /usr/local/lib/libluajit-5.1.so

VG=" --leak-check=full --show-leak-kinds=all --track-origins=yes "
valgrind $VG ./test_csp_parse csp1.html _csp_1.html 1>_out 2>_err
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
# rm -f _*
