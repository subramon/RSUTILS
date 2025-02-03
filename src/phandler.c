#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "web_struct.h"
#include "alt_handler.h"
#include "phandler.h"
// phandler invoked by evhttp callback 
void
phandler(
    struct evhttp_request *req,
    void *arg // same as web_info from evhttp_set_gencb()
    )
{
  int status = 0;
  web_info_t *winfo = (web_info_t *)arg;
  pthread_t *workers = winfo->workers;
  int *wstat = winfo->wstat; // worker status
  int n_workers = winfo->n_workers;

  // Look for a free worker
  int worker_id = -1;
  for ( int i = 0; i < n_workers; i++ ) { 
    int l_expected = 0;
    int l_desired  = 2;
    bool rslt = __atomic_compare_exchange(&(wstat[i]),
        &l_expected, &l_desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if ( rslt ) { worker_id = i; break; }
  }
  if ( worker_id < 0 ) {  // no workers available, create new one 

  pthread_attr_t attr;
  status = pthread_attr_init(&attr); cBYE(status);
  int ds = PTHREAD_CREATE_DETACHED; 
  status = pthread_attr_setdetachstate(&attr, ds); cBYE(status);
  pthread_t handler_thrd;
  core_lhandler_arg_t core_arg; 
  core_arg.req = req; 
  core_arg.arg = arg;
  status = pthread_create(&handler_thrd, &attr, alt_handler, &core_arg);
  cBYE(status);
  }
  else {
    printf("TODO\n");
    go_BYE(-1);
  }
BYE:
  if ( status < 0 ) { WHEREAMI; } 
}
