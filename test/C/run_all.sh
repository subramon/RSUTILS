#!/bin/bash
set -e
export RSUTILS_SRC_ROOT=$HOME/RSUTILS/
bash test_file_exists.sh
bash test_get_bit_u64.sh
bash test_get_file_size.sh
bash test_isdir.sh
bash test_isfile.sh
bash test_rand_file_name.sh
bash test_trim.sh

echo "Completed $0 in $PWD
