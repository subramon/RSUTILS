#!/bin/bash


CC = gcc 
INCS=-I../inc/ -I../inc/
INCDIR=../inc/

MYSO = librsutils.so

all : ${MYSO}  ut_task_cpu_time \
	../lua/csv_meta_t.lua  \
	../lua/csv_spec_t.lua   \
	../lua/qtypes_def.lua  \
	../lua/web_info.lua  \
	webserver.o \
	hit_endp.o \
	setup_curl.o \
	execute_curl.o \
	cli_hit_endp \
	cli_asc_to_bin \


gen_inc_files : \
	../inc/txt_to_BL.h \
	../inc/txt_to_SC.h \
	../inc/txt_to_I1.h \
	../inc/txt_to_I2.h \
	../inc/txt_to_I4.h \
	../inc/txt_to_I8.h \
	../inc/txt_to_UI1.h \
	../inc/txt_to_UI2.h \
	../inc/txt_to_UI4.h \
	../inc/txt_to_UI8.h \
	../inc/txt_to_F4.h \
	../inc/txt_to_F8.h \
	../inc/BL_to_txt.h \
	../inc/SC_to_txt.h \
	../inc/I1_to_txt.h \
	../inc/I2_to_txt.h \
	../inc/I4_to_txt.h \
	../inc/I8_to_txt.h \
	../inc/F4_to_txt.h \
	../inc/F8_to_txt.h \
	../inc/file_as_str.h \
	../inc/file_exists.h \
	../inc/isfile.h \
	../inc/isfile_in_dir.h \
	../inc/isdir.h \
	../inc/rand_file_name.h \
	../inc/get_bit_u64.h \
	../inc/get_file_size.h \
	../inc/mem_info.h \
	../inc/get_time_usec.h \
	../inc/get_bit_u64.h \
	../inc/is_valid_chars_for_num.h \
	../inc/mk_file.h \
	../inc/mk_dir_file_name.h \
	../inc/q_omp_get_num_procs.h \
	../inc/rdtsc.h \
	../inc/rs_mmap.h \
	../inc/set_bit_u64.h \
	../inc/str_as_file.h \
	../inc/split_str.h \
	../inc/stretch.h \
	../inc/tm2time.h  \
	../inc/trim.h 

#----------------------------------

../inc/txt_to_BL.h : 
	luajit ../src/cli_extract_func_decl.lua txt_to_BL.c ${INCDIR}
../inc/txt_to_SC.h :
	luajit ../src/cli_extract_func_decl.lua txt_to_SC.c ${INCDIR}
txt_to_BL.c : ../inc/txt_to_BL.h
txt_to_SC.c : ../inc/txt_to_SC.h
#----------------------------------
../inc/txt_to_I1.h :
	luajit gen_code_I.lua 
../inc/txt_to_I2.h :
	luajit gen_code_I.lua 
../inc/txt_to_I4.h :
	luajit gen_code_I.lua 
../inc/txt_to_I8.h :
	luajit gen_code_I.lua 
../inc/txt_to_F4.h :
	luajit gen_code_F.lua 
../inc/txt_to_F8.h :
	luajit gen_code_F.lua 
#----------------------------------
../src/txt_to_I1.c :
	luajit gen_code_I.lua 
../src/txt_to_I2.c :
	luajit gen_code_I.lua 
../src/txt_to_I4.c :
	luajit gen_code_I.lua 
../src/txt_to_I8.c :
	luajit gen_code_I.lua 
../src/txt_to_F4.c :
	luajit gen_code_F.lua 
../src/txt_to_F8.c :
	luajit gen_code_F.lua 
#----------------------------------
../inc/BL_to_txt.h : 
	lua ../src/cli_extract_func_decl.lua BL_to_txt.c ${INCDIR}
../inc/SC_to_txt.h : 
	lua ../src/cli_extract_func_decl.lua SC_to_txt.c ${INCDIR}
BL_to_txt.c : ../inc/BL_to_txt.h
SC_to_txt.c : ../inc/SC_to_txt.h
#----------------------------------
../inc/I1_to_txt.h :
	luajit print_generator.lua I1
../inc/I2_to_txt.h :
	luajit print_generator.lua I2
../inc/I4_to_txt.h :
	luajit print_generator.lua I4
../inc/I8_to_txt.h :
	luajit print_generator.lua I8
../inc/F4_to_txt.h :
	luajit print_generator.lua F4
../inc/F8_to_txt.h :
	luajit print_generator.lua F8
#----------------------------------
../src/I1_to_txt.c :
	luajit print_generator.lua I1
../src/I2_to_txt.c :
	luajit print_generator.lua I2
../src/I4_to_txt.c :
	luajit print_generator.lua I4
../src/I8_to_txt.c :
	luajit print_generator.lua I8
../src/F4_to_txt.c :
	luajit print_generator.lua F4
../src/F8_to_txt.c :
	luajit print_generator.lua F8

../inc/file_as_str.h : 
	luajit ../src/cli_extract_func_decl.lua file_as_str.c ${INCDIR}

../inc/file_exists.h : 
	luajit ../src/cli_extract_func_decl.lua file_exists.c ${INCDIR}

../inc/isfile.h :
	luajit ../src/cli_extract_func_decl.lua isfile.c ${INCDIR}

../inc/isfile_in_dir.h :
	luajit ../src/cli_extract_func_decl.lua isfile_in_dir.c ${INCDIR}

../inc/isdir.h :
	luajit ../src/cli_extract_func_decl.lua isdir.c ${INCDIR}

../inc/rand_file_name.h : 
	luajit ../src/cli_extract_func_decl.lua rand_file_name.c ${INCDIR}

../inc/get_file_size.h : 
	luajit ../src/cli_extract_func_decl.lua get_file_size.c ${INCDIR}

../inc/mem_info.h : 
	luajit ../src/cli_extract_func_decl.lua mem_info.c ${INCDIR}

../inc/get_time_usec.h : 
	luajit ../src/cli_extract_func_decl.lua get_time_usec.c ${INCDIR}
  
../inc/get_bit_u64.h :
	luajit ../src/cli_extract_func_decl.lua get_bit_u64.c ${INCDIR}

../inc/is_valid_chars_for_num.h : 
	luajit ../src/cli_extract_func_decl.lua is_valid_chars_for_num.c ${INCDIR}

../inc/mk_file.h :
	luajit ../src/cli_extract_func_decl.lua mk_file.c ${INCDIR}

../inc/mk_dir_file_name.h :
	luajit ../src/cli_extract_func_decl.lua mk_dir_file_name.c ${INCDIR}

../inc/q_omp_get_num_procs.h :
	luajit ../src/cli_extract_func_decl.lua q_omp_get_num_procs.c ${INCDIR}

../inc/rdtsc.h : 
	luajit ../src/cli_extract_func_decl.lua rdtsc.c ${INCDIR}

../inc/rs_mmap.h :
	luajit ../src/cli_extract_func_decl.lua rs_mmap.c ${INCDIR}

../inc/set_bit_u64.h :
	luajit ../src/cli_extract_func_decl.lua set_bit_u64.c ${INCDIR}

../inc/str_as_file.h :
	luajit ../src/cli_extract_func_decl.lua str_as_file.c ${INCDIR}

../inc/stretch.h :
	luajit ../src/cli_extract_func_decl.lua stretch.c ${INCDIR}

../inc/trim.h : 
	luajit ../src/cli_extract_func_decl.lua trim.c ${INCDIR}

../inc/tm2time.h : 
	luajit ../src/cli_extract_func_decl.lua tm2time.c ${INCDIR}

GEN_INC_FILES = \
	../inc/BL_to_txt.h   \
	../inc/file_as_str.h  \
	../inc/file_exists.h  \
	../inc/get_bit_u64.h \
	../inc/get_file_size.h  \
	../inc/get_time_usec.h \
	../inc/isdir.h  \
	../inc/isfile.h  \
	../inc/isfile_in_dir.h  \
	../inc/is_valid_chars_for_num.h  \
	../inc/mem_info.h  \
	../inc/mk_file.h \
	../inc/mk_dir_file_name.h \
	../inc/q_omp_get_num_procs.h \
	../inc/rand_file_name.h  \
	../inc/rdtsc.h \
	../inc/rs_mmap.h \
	../inc/SC_to_txt.h   \
	../inc/set_bit_u64.h \
	../inc/str_as_file.h \
	../inc/stretch.h \
	../inc/trim.h  \
#--------

lookup8.o : QCFLAGS += -Wno-implicit-fallthrough # not my code :-(

.c.o :
	$(CC) -c -o $@ $< $(QCFLAGS)  $(INCS)


spooky_hash.o : QCFLAGS += -Wno-implicit-fallthrough -Wno-cast-qual

SRCS := addr_as_hex_string.c 
SRCS += BL_to_txt.c 
SRCS += bf16tofp32.c
SRCS += bin_search_UI4.c
SRCS += cat_to_buf.c 
SRCS += copy_file.c 
SRCS += csv_meta_aux.c 
SRCS += csv_spec_aux.c 
SRCS += extract_api_args.c 
SRCS += extract_extension.c 
SRCS += extract_name_value.c 
SRCS += extract_json_value.c 
SRCS += file_cat.c 
SRCS += file_as_str.c 
SRCS += file_exists.c 
SRCS += fp32tobf16.c
SRCS += free_2d_array.c 
SRCS += get_bit_u64.c  
SRCS += get_body.c 
SRCS += get_time_usec.c 
SRCS += get_file_size.c 
SRCS += gsub.c 
SRCS += html_escape.c 
SRCS += is_valid_chars_for_num.c 
SRCS += isfile.c 
SRCS += isfile_in_dir.c 
SRCS += isdir.c 
SRCS += line_finder.c
SRCS += num_lines_safe.c
SRCS += lookup8.c
SRCS += ls.c
SRCS += mk_dir_file_name.c 
SRCS += mk_file.c 
SRCS += mem_info.c 
SRCS += mk_temp_copy.c
SRCS += mk_temp_file.c
SRCS += mk_temp_file_name.c
SRCS += multiple.c 
SRCS += num_cols.c
SRCS += num_lines.c
SRCS += set_http_headers.c
SRCS += parse_json_array.c  
SRCS += q_omp_get_num_procs.c  
SRCS += qtypes.c 
SRCS += queue_helpers.c  
SRCS += rand_file_name.c 
SRCS += rdtsc.c 
SRCS += read_csv.c
SRCS += read_to_eoln.c
SRCS += rmtree.c 
SRCS += rs_mmap.c 
SRCS += rs_strcat.c 
SRCS += SC_to_txt.c 
SRCS += send_udp_pkt.c  
SRCS += set_bit_u64.c  
SRCS += sort1.c 
SRCS += split_str.c
SRCS += spooky_hash.c 
SRCS += str_as_file.c  
SRCS += stretch.c  
SRCS += strnstr.c  
SRCS += str_between.c  
SRCS += swap_bytes.c  
SRCS += task_cpu_time.c
SRCS += proc_info.c
SRCS += tm2time.c 
SRCS += touch.c 
SRCS += trim.c 
SRCS += txt_to_BL.c 
SRCS += txt_to_SC.c 
SRCS += unique.c
SRCS += url.c
SRCS += I4_to_TM1.c

OBJS = $(SRCS:.c=.o)

GEN_SRCS := ../src/I1_to_txt.c  
GEN_SRCS += ../src/I2_to_txt.c  
GEN_SRCS += ../src/I4_to_txt.c 
GEN_SRCS += ../src/I8_to_txt.c
GEN_SRCS += ../src/F4_to_txt.c
GEN_SRCS += ../src/F8_to_txt.c
GEN_SRCS += ../src/txt_to_I1.c
GEN_SRCS += ../src/txt_to_I2.c
GEN_SRCS += ../src/txt_to_I4.c
GEN_SRCS += ../src/txt_to_I8.c
GEN_SRCS += ../src/txt_to_UI1.c
GEN_SRCS += ../src/txt_to_UI2.c
GEN_SRCS += ../src/txt_to_UI4.c
GEN_SRCS += ../src/txt_to_UI8.c
GEN_SRCS += ../src/txt_to_F4.c
GEN_SRCS += ../src/txt_to_F8.c

GEN_OBJS = $(GEN_SRCS:.c=.o)
#----------------------------------

read_csv.o : QCFLAGS += -Wno-cast-align # for ARM

${MYSO} : ${GEN_OBJS} ${OBJS}  
	gcc -shared ${QCFLAGS} ${QLDFLAGS} ${INCS} \
		${GEN_OBJS} ${OBJS} -o ${MYSO} -levent -latomic -ljansson

ut_task_cpu_time : ut_task_cpu_time.c
	gcc -I../inc/ ut_task_cpu_time.c -o ut_task_cpu_time ${MYSO} -lpthread -lm
clean :
	rm -f _*
	rm -f *.so
	rm -f *.o
	rm -f  ${MYSO}
	rm -f cli_asc_to_bin 
	rm -f cli_hit_endp 


../lua/web_info.lua : ${RSUTILS_SRC_ROOT}/inc/web_struct.h
	echo " return [[ " > _xxx
	luajit ${RSUTILS_SRC_ROOT}/lua/cli_for_cdef.lua  \
	${RSUTILS_SRC_ROOT}/inc/web_struct.h >> _xxx
	echo " ]] " >> _xxx
	cat _xxx | sed s'/lua_State/void/'g > ../lua/web_info.lua
	rm _xxx

../lua/csv_meta_t.lua : ${RSUTILS_SRC_ROOT}/inc/qtypes.h
	echo " return [[ " > ../lua/csv_meta_t.lua 
	luajit ${RSUTILS_SRC_ROOT}/lua/cli_for_cdef.lua  \
		${RSUTILS_SRC_ROOT}/inc/csv_meta_t.h \
		>> ../lua/csv_meta_t.lua 
	echo " ]] " >> ../lua/csv_meta_t.lua 

../lua/csv_spec_t.lua : ${RSUTILS_SRC_ROOT}/inc/qtypes.h
	echo " return [[ " > ../lua/csv_spec_t.lua 
	luajit ${RSUTILS_SRC_ROOT}/lua/cli_for_cdef.lua  \
		${RSUTILS_SRC_ROOT}/inc/csv_spec_t.h \
		>> ../lua/csv_spec_t.lua 
	echo " ]] " >> ../lua/csv_spec_t.lua 

../lua/qtypes_def.lua : ${RSUTILS_SRC_ROOT}/inc/qtypes.h
	echo " return [[ " > ../lua/qtypes_def.lua 
	luajit ${RSUTILS_SRC_ROOT}/lua/cli_for_cdef.lua  \
		${RSUTILS_SRC_ROOT}/inc/qtypes.h \
		>> ../lua/qtypes_def.lua 
	echo " ]] " >> ../lua/qtypes_def.lua 

cli_asc_to_bin :  cli_asc_to_bin.c
	gcc ${QCFLAGS} ${INCS} cli_asc_to_bin.c -o cli_asc_to_bin librsutils.so

cli_hit_endp :  \
	cli_hit_endp.o \
	hit_endp.o \
	isfile.o \
	setup_curl.o  \
	execute_curl.o  
	gcc \
	cli_hit_endp.o \
	hit_endp.o \
	isfile.o \
	setup_curl.o  \
	execute_curl.o  \
		-o cli_hit_endp /usr/local/lib/libcurl.so

# do not use following unless you absolutely need to
# else, it breaks the build from UTILS/build/
hardclean :
	rm -f ${GEN_SRCS} ${GEN_INC_FILES}
	rm -f  \
	../lua/csv_meta.lua  \
	../lua/csv_spec.lua   \
	../lua/qtypes_def.lua  \
	../lua/web_info.lua  \


