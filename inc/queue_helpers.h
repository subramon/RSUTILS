extern int
mark_write_done(
    int *ptr_q_loc
    );
extern int
release_read(
    int *ptr_q_loc
    );
extern int
mark_read_done(
    int *ptr_q_loc
    );
extern void
wait_for_spot(
    int *q_loc, 
    int qidx, 
    int expected,
    int desired,
    uint64_t max_t_sleep,
    uint64_t *ptr_t_sleep
    );
extern bool
is_all_done(
    _Atomic int *ptr_prod_done, // 1 if producer is done, 0 otherwise
    uint64_t *ptr_num_produced,
    uint64_t *ptr_num_consumed
    );
extern int
get_free_spot(
    int *q_loc,  // [nQ]
    uint32_t nQ,
    int expected,
    int desired,
    uint64_t max_t_sleep,
    uint64_t *ptr_t_sleep
    );
extern int 
acquire_lock(
    _Atomic int *ptr_lock,
    int expected,
    int desired,
    int max_wait_msec,
    bool *ptr_lock_got
    );
extern int
release_lock(
    _Atomic int *ptr_lock,
    int expected,
    int desired
    );
extern int
acquire_lock64(
    _Atomic uint64_t *ptr_lock,
    uint64_t expected,
    uint64_t desired,
    int max_wait_msec,
    bool *ptr_lock_got
    );
extern int
release_lock64(
    _Atomic uint64_t *ptr_lock,
    uint64_t expected,
    uint64_t desired
    );
extern int
lock64(
    _Atomic uint64_t *ptr_lock
    );
extern int
unlock64(
    _Atomic uint64_t *ptr_lock
    );
