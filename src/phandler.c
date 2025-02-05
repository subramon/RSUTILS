#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "web_struct.h"
#include "alt_handler.h"
#include "phandler.h"
// phandler invoked by evhttp callback 
// WARNING! THIS DID NOT WORK BECAUSE WE WERE UNABLE TO PREVENT
// libevent from freeing the ebvhttp_request even when we had taken
// ownership of it and were explicitly freeing it in handler()
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
  evhttp_request_own(req); // TODO P0 EXPERIMENTAL 
  printf("Taking ownership of req\n");
  if ( evhttp_request_is_owned(req) ) { 
    fprintf(stderr, "Taken ownership of req\n");
  }
  status = pthread_create(&handler_thrd, &attr, alt_handler, &alt_arg);
  cBYE(status);
BYE:
  if ( status < 0 ) { WHEREAMI; } 
}
