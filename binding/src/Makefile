
INCS= -I../../inc  -I/usr/include/lua5.1 \


all: libcutils.so 

SRCS := cutils.c 
SRCS += ../../src/qtypes.c 
SRCS += ../../src/file_exists.c 
SRCS += ../../src/cat_to_buf.c 
SRCS += ../../src/multiple.c 
SRCS += ../../src/file_as_str.c 
SRCS += ../../src/file_cat.c 
SRCS += ../../src/get_bit_u64.c 
SRCS += ../../src/get_file_size.c 
SRCS += ../../src/get_time_usec.c 
SRCS += ../../src/isdir.c 
SRCS += ../../src/rmtree.c 
SRCS += ../../src/isfile.c 
SRCS += ../../src/isfile_in_dir.c 
SRCS += ../../src/line_breaks.c 
SRCS += ../../src/ls.c 
SRCS += ../../src/mem_info.c 
SRCS += ../../src/mk_file.c 
SRCS += ../../src/mk_temp_file.c 
SRCS += ../../src/mk_temp_file_name.c 
SRCS += ../../src/num_lines.c 
SRCS += ../../src/num_cols.c 
SRCS += ../../src/rand_file_name.c 
SRCS += ../../src/rdtsc.c 
SRCS += ../../src/rs_mmap.c 
SRCS += ../../src/str_as_file.c 
SRCS += ../../src/tm2time.c 

OBJS = $(SRCS:.c=.o)

.c.o : 
	$(CC) -c -o $@ $< ${QCFLAGS} $(INCS) 

cutils.o : QCFLAGS += -Wno-cast-qual
clean:
	rm -f *.so *.o _* ${OBJS} 
		
libcutils.so:   ${OBJS} 
	gcc ${OBJS} -shared -o libcutils.so -lgomp ${QLDFLAGS}

