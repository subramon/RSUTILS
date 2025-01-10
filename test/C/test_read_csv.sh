#!/bin/bash
set -e 
rm -f _out _err
gcc  -g test_read_csv.c ../../src/line_finder.c ../../src/num_lines_safe.c ../../src/multiple.c ../../src/rs_mmap.c ../../src/cat_to_buf.c ../../src/num_lines.c ../../src/qtypes.c ../../src/read_csv.c -std=gnu99 -I../../inc/ -I../../gen_inc/ 
valgrind ./a.out ./data/data_1_test_read_csv.csv 1>_out 2>_err
grep  -v "definitely lost" _err 1>/dev/null
grep  "0 errors from 0 contexts" _err 1>/dev/null
grep  "SUCCESS" _out 1>/dev/null
echo "Successfully completed $0 in $PWD"
# rm -f _*
