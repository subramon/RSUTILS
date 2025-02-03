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

  pthread_attr_t attr; status = pthread_attr_init(&attr); cBYE(status);
  int ds = PTHREAD_CREATE_DETACHED; 
  status = pthread_attr_setdetachstate(&attr, ds); cBYE(status);
  pthread_t handler_thrd;
  alt_handler_arg_t alt_arg; 
  alt_arg.req = req; 
  alt_arg.arg = arg;
  status = pthread_create(&handler_thrd, &attr, alt_handler, &alt_arg);
  cBYE(status);
BYE:
  if ( status < 0 ) { WHEREAMI; } 
}
