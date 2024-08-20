extern void
wait_for_spot(
    int *q_loc, 
    int qidx, 
    int expected,
    int desired,
    uint64_t *ptr_t_sleep
    );
extern bool
is_all_done(
    int *ptr_prod_done, // 1 if producer is done, 0 otherwise
    uint64_t *ptr_num_produced,
    uint64_t *ptr_num_consumed
    );