#include <pthread.h>
#include <event.h>
#include <evhttp.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
//------ above from example
#include "q_incs.h"
#include <evhttp.h>
#include <event2/thread.h>
#include <pthread.h>
#include "web_struct.h"
#include "phandler.h"
#include "handler.h"
#include "webserver.h"

static void* dispatch(
    void *arg
    ) 
{
  event_base_dispatch((struct event_base*)arg);
  return NULL;
}

static int 
bind_socket(
    int port
    ) 
{
  int r;
  int nfd;
  nfd = socket(AF_INET, SOCK_STREAM, 0);
  if (nfd < 0) return -1;
  int one = 1;
  r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
  if (r < 0) return -1;
  r = listen(nfd, 10240);
  if (r < 0) return -1;
  int flags;
  if ( ((flags = fcntl(nfd, F_GETFL, 0)) < 0 ) 
      || ( fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0) ) { 
    return -1;
  }
  // TODO Bind to 0.0.0.0 or 127.0.0.1
  return nfd;
}

__attribute__((noreturn))
void *
webserver(
    void *arg
    )
{
  int status;
  web_info_t *web_info = (web_info_t *)arg;
  int nthreads = 4; 
  pthread_t ths[nthreads];

  if ( arg == NULL ) { go_BYE(-1); }

  status = evthread_use_pthreads();
  int port = web_info->port; 
  if ( ( port <= 0 )  || ( port > 65535 ) ) { go_BYE(-1); } 

  int nfd = bind_socket(port);
  if ( nfd < 0 ) { go_BYE(-1); }

  for ( int i = 0; i < nthreads; i++ ) {
    struct event_base *base = event_init();
    if ( base == NULL ) { go_BYE(-1); }
    struct evhttp *httpd  = evhttp_new(base);
    if ( httpd == NULL ) { go_BYE(-1); }

    //---------------------------------------------
    uint32_t sz = web_info->max_headers_size;
    if ( sz == 0 ) { sz = 4096; } 
    evhttp_set_max_headers_size(httpd, sz); 
    //---------------------------------------------
    sz = web_info->max_body_size;
    if ( sz == 0 ) { sz = 1048576; } 
    evhttp_set_max_body_size(httpd, sz); 
    //---------------------------------------------

    status = evhttp_accept_socket(httpd, nfd); cBYE(status);
    evhttp_set_gencb(httpd, handler, web_info);
    status = pthread_create(&(ths[i]), NULL, dispatch, base); cBYE(status);
    printf("Forked thread %d \n", i);
  }
  /* TODO P1 WHAT TO DO ABOUT THE GUYS? 
  evhttp_free(httpd);
  event_base_free(base);
  */

  for (int i = 0; i < nthreads; i++) {
    pthread_join(ths[i], NULL);
  }
  libevent_global_shutdown();

BYE:
  fprintf(stdout, "Web Server terminating\n");
  pthread_exit(NULL);
}
