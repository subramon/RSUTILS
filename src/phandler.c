#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
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
BYE:
  if ( status < 0 ) { WHEREAMI; } 
}
