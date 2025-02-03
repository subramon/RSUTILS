#include <pthread.h>
#include <evhttp.h>
#include "event2/http.h"
#include "q_incs.h"
#include "handler.h"
#include "loop_handler.h"
void *
loop_handler(
    void *in_loop_arg
    )
{
  loop_arg_t *loop_arg = (loop_arg_t *)in_loop_arg;
  handler(alt_arg->req, alt_arg->arg);
  pthread_exit(NULL);
}
