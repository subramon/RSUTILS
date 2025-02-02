#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "handler.h"
#include "alt_handler.h"
void *
alt_handler(
    void *in_alt_arg
    )
{
  core_lhandler_arg_t *alt_arg = (core_lhandler_arg_t *)in_alt_arg;
  handler(alt_arg->req, alt_arg->arg);
  pthread_exit(NULL);
}
