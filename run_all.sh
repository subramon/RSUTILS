source to_source
test -d $RSUTILS_SRC_ROOT
cd binding/test/
luajit test_cutils.lua
cd -
#----------------------
cd test/C/
bash run_all.sh
echo "Successfully completed $0 in $PWD"