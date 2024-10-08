#include "q_incs.h"
#include "get_time_usec.h"
#include "queue_helpers.h"
int
mark_write_done(
    int *ptr_q_loc
    )
{
  int status = 0;
  int l_expected = 1, l_desired = 2;
  bool rslt = __atomic_compare_exchange(ptr_q_loc,
      &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  if ( !rslt ) { go_BYE(-1); }
BYE:
  return status;
}

int
release_read(
    int *ptr_q_loc
    )
{
  int status = 0;
  int l_expected = 3, l_desired = 2;
  bool rslt = __atomic_compare_exchange(ptr_q_loc,
      &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  if ( !rslt ) { go_BYE(-1); }
BYE:
  return status;
}

int
mark_read_done(
    int *ptr_q_loc
    )
{
  int status = 0;
  int l_expected = 3, l_desired = 0;
  bool rslt = __atomic_compare_exchange(ptr_q_loc,
      &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  if ( !rslt ) { go_BYE(-1); }
BYE:
  return status;
}

void
wait_for_spot(
    int *q_loc, 
    int qidx, 
    int expected,
    int desired,
    uint64_t max_t_sleep,
    uint64_t *ptr_t_sleep
    )
{
  uint64_t l_t_sleep = 0;
  for ( ; ; ) {
    int l_expected = expected;
    int l_desired  = desired;
    bool rslt = __atomic_compare_exchange(
        &(q_loc[qidx]), &l_expected, &l_desired, false, 
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    // if spot is busy, then skip to next spot
    if ( rslt ) { 
      // printf("Placed data in position %u\n", qidx);
      break; 
    }
    // wait a bit for an empty spot 
    uint64_t t1 = get_time_usec();
    struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    uint64_t t2 = get_time_usec();
    *ptr_t_sleep += (t2-t1);
    l_t_sleep += (t2-t1);
    if ( ( max_t_sleep > 0 ) && ( l_t_sleep > max_t_sleep ) ) { break; }
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
int
get_free_spot(
    int *q_loc,  // [nQ]
    uint32_t nQ,
    int expected,
    int desired,
    uint64_t max_t_sleep,
    uint64_t *ptr_t_sleep
    )
{
  uint64_t l_t_sleep = 0;
  if ( q_loc == NULL ) { return -1; }
  if ( nQ == 0 ) { return -1; }
  if ( expected == desired ) { return -1; }
  bool found = false;
  int qidx = -1;
  for ( ; ; ) {
    for ( uint32_t i = 0; i < nQ; i++ ) { 
      int l_expected = expected;
      int l_desired  = desired;
      bool rslt = __atomic_compare_exchange(
          &(q_loc[i]), &l_expected, &l_desired, false, 
          __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
      // if spot is busy, then skip to next spot
      if ( rslt ) { 
        found = true; qidx = i; break; 
      }
    }
    if ( found ) { break; }
    // wait a bit for an empty spot 
    uint64_t t1 = get_time_usec();
    struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    uint64_t t2 = get_time_usec();
    *ptr_t_sleep += (t2-t1);
    l_t_sleep += (t2-t1);
    if ( ( max_t_sleep > 0 ) && ( l_t_sleep > max_t_sleep ) ) { break; }
  }
  return qidx;
}

int
acquire_lock(
    int *ptr_lock,
    int expected,
    int desired,
    int max_wait_msec,
    bool *ptr_lock_got
    )
{
  int status = 0;
  int t_sleep = 0; 

  *ptr_lock_got = false;
  for ( ; ; ) { 
    bool rslt = __atomic_compare_exchange(
        ptr_lock, &expected, &desired, false, 
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( rslt ) { 
      *ptr_lock_got = true;
      break;
    }
    else { // wait a bit and try again
      uint64_t t1 = get_time_usec();
      struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 1000000 };
      nanosleep(&tmspec, NULL);
      uint64_t t2 = get_time_usec();
      t_sleep += (t2-t1) / 1000; 
      if ( ( max_wait_msec > 0 ) && ( t_sleep > max_wait_msec ) ) {
        *ptr_lock_got = false;
        break;
      }
    }
  }
BYE:
  return status;
}
int
release_lock(
    int *ptr_lock,
    int expected,
    int desired
    )
{
  int status = 0;
  bool rslt = __atomic_compare_exchange(
        ptr_lock, &expected, &desired, false, 
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  if ( !rslt ) { go_BYE(-1); }
BYE:
  return status;
}
