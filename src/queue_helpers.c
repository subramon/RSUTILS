#include "q_incs.h"
#include "get_time_usec.h"
#include "queue_helpers.h"
void
wait_for_spot(
    int *q_loc, 
    int qidx, 
    int expected,
    int desired,
    uint64_t *ptr_t_sleep
    )
{
  for ( ; ; ) {
    int l_expected = expected;
    int l_desired  = desired;
    bool rslt = __atomic_compare_exchange(
        &(q_loc[qidx]), &l_expected, &l_desired, false, 
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    // if spot is busy, then skip to next spot
    if ( rslt ) { 
      printf("Placed data in position %u\n", qidx);
      break; 
    }
    // wait a bit for an empty spot 
    uint64_t t1 = get_time_usec();
    struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    uint64_t t2 = get_time_usec();
    *ptr_t_sleep += (t2-t1);
  }
}

bool
is_all_done(
    int *ptr_prod_done, // 1 if producer is done, 0 otherwise
    uint64_t *ptr_num_produced,
    uint64_t *ptr_num_consumed
    )
{

  // find out if surveyor is done 
  int prod_done;
  __atomic_load(ptr_prod_done, &prod_done, 0);
  // find out if you have read everything surveyor has written
  uint64_t num_produced;
  uint64_t num_consumed;
  __atomic_load(ptr_num_produced, &num_produced, 0);
  __atomic_load(ptr_num_consumed, &num_consumed, 0);
  if ( ( prod_done == 1 ) && 
      ( num_produced == num_consumed ) ) { 
    return true;
  }
  else {
    return false;
  }
}
