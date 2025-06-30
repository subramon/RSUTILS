#include <stdint.h>
extern pid_t 
get_task_id(
    void
    );
extern int 
get_task_cpu_time(
    pid_t tid
    );
extern int 
get_utime_stime_for_tid(
    pid_t tid,
    uint64_t *ptr_utime,
    uint64_t *ptr_stime
    ) ;
