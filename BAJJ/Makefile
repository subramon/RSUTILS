# CFLAGS := -O0 # optimizations cause buffer overflow on maas
CFLAGS := -g -O0 # optimizations cause buffer overflow on maas
# CFLAGS := -O3

CFLAGS += -DDEBUG
CFLAGS += -fopenmp
CFLAGS += -fstrict-aliasing 
CFLAGS += -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align 
CFLAGS += -Wstrict-aliasing
# CFLAGS += -Wincompatible-pointer-types # comment for CentOS
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings 
CFLAGS += -Wunused-variable -Wunused-parameter 
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith 
CFLAGS += -Wmissing-declarations -Wredundant-decls -Wnested-externs 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings 
CFLAGS += -Wold-style-definition 
CFLAGS += -Wattributes
CFLAGS += -Wunused-label 
CFLAGS += -Wconversion
CFLAGS += -Wuninitialized
# example: rbc_mgcv_model.o : CFLAGS += -Wno-cast-qual

INCS += -I${RSUTILS_SRC_ROOT}/inc/ 
LIBS := -lm 
LIBS += -lpthread 

MYLIBS += ${RSUTILS_SRC_ROOT}/src/librsutils.so 

all: make_data \
  make_dist \
  calc_dist

CC=gcc
# Use clang as an extra compile time check
# CC=clang
# CFLAGS=-Weverything -Wno-switch-enum

.c.o :
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

make_data : make_data.o 
	gcc make_data.o -o make_data ${MYLIBS}

make_dist : make_dist.o 
	gcc make_dist.o -o make_dist ${MYLIBS}

calc_dist : calc_dist.o 
	gcc calc_dist.o -o calc_dist ${MYLIBS}


clean :
	rm -f make_data make_dist calc_dist \
	rm -f *.o
