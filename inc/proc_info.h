#include <stdint.h>
#ifndef _PROC_INFO_H
#define  _PROC_INFO_H
typedef struct _proc_info_t {
  unsigned long utime; // 14 
  unsigned long stime; // 15 
  long cutime; // 16
  long cstime; // 17
  unsigned long long starttime; // 22
  unsigned long vsize; // 23
  long rss; // 24
} proc_info_t;

extern char *
proc_info_as_json(
    proc_info_t *ptr_info
    );
extern int
get_proc_info_for_tid(
    pid_t tid,
    proc_info_t *ptr_info
    ) ;
#endif //  _PROC_INFO_H

/*
(16) cutime  %ld
Amount of time that this process's waited-for chil‐
dren have been scheduled in user mode, measured in
clock ticks (divide by sysconf(_SC_CLK_TCK)).  (See
also times(2).)  This includes guest time,
cguest_time (time spent running a virtual CPU, see
below).

(17) cstime  %ld
Amount of time that this process's waited-for chil‐
dren have been scheduled in kernel mode, measured in
clock ticks (divide by sysconf(_SC_CLK_TCK)).

(22) starttime  %llu
The time the process started after system boot.  In
kernels before Linux 2.6, this value was expressed
in jiffies.  Since Linux 2.6, the value is expressed
in clock ticks (divide by sysconf(_SC_CLK_TCK)).

The format for this field was %lu before Linux 2.6.

(23) vsize  %lu
Virtual memory size in bytes.

(24) rss  %ld
Resident Set Size: number of pages the process has
in real memory.  This is just the pages which count
toward text, data, or stack space.  This does not
include pages which have not been demand-loaded in,
or which are swapped out.



*/

