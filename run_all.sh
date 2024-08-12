#!/bin/bash
set -e 
source to_source
test -d $RSUTILS_SRC_ROOT
cd $RSUTILS_SRC_ROOT/binding/test/
luajit test_cutils.lua
#TODO Add all the other test_*.lua 
cd $RSUTILS_SRC_ROOT/test/Lua/
luajit test_tbl_to_C_2d.lua
#TODO Add all the other test_*.lua 
#----------------------
cd ${RSUTILS_SRC_ROOT}/test/C/
bash run_all.sh
echo "Successfully completed $0 in $PWD"
