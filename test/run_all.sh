#!/bin/bash
set -e
bash test_bytes_to_bits.sh
bash test_copy_file.sh
bash test_file_exists.sh
bash test_get_bit_u64.sh
bash test_get_file_size.sh
bash test_isdir.sh
bash test_isfile.sh
bash test_line_breaks.sh
bash test_rand_file_name.sh
bash test_trim.sh
bash test_stretch.sh

echo "Completed $0 in $PWD
