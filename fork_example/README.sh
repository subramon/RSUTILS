#!/bin/bash
set -e 
# Purpose of this code is to demonstrate that when multiple processes 
# are forked, the death of some of the child processes does not impact 
# the success of the others.

test -d ${RSUTILS_SRC_ROOT}
gcc child.c -o child -I${RSUTILS_SRC_ROOT}/inc/
gcc parent.c -o parent -I${RSUTILS_SRC_ROOT}/inc/
export PATH=$PATH:$PWD
./parent

