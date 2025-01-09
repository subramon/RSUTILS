#!/bin/bash
set -e
cd ../..
source to_source
cd -
test -d $RSUTILS_SRC_ROOT
# echo "RSUTILS_SRC_ROOT=$RSUTILS_SRC_ROOT"
bash test_file_exists.sh
bash test_stretch.sh
bash test_get_bit_u64.sh
bash test_get_file_size.sh
bash test_isdir.sh

bash test_isfile.sh
bash test_rand_file_name.sh
bash test_trim.sh

bash test_transpose.sh

echo "Completed $0 in $PWD"
