gcc -g -I../RSUTILS/inc/ \
  make_data.c \
  ../RSUTILS/src/multiple.c \
  -o mk_data
gcc -g -I../RSUTILS/inc/  \
  make_dist.c  \
  ../RSUTILS/src/mk_file.c \
  ../RSUTILS/src/multiple.c \
  -o mk_dist
gcc -g -I../RSUTILS/inc/  \
  calc_dist.c  \
  ../RSUTILS/src/rs_mmap.c \
  ../RSUTILS/src/multiple.c \
  -o cl_dist
