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
#include "handler.h"
#include "webserver.h"

static void* 
dispatch(
    void *arg
    ) 
{
  event_base_dispatch((struct event_base*)arg);
  return NULL;
}

static int 
bind_socket(
    int port,
    int *ptr_nfd
    ) 
{
  int status = 0;
  int r;
  int nfd;
  nfd = socket(AF_INET, SOCK_STREAM, 0);
  if (nfd < 0) { go_BYE(-1); } 
  int one = 1;
  r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  if ( ( port < 0 ) || ( port > 65535 ) ) { go_BYE(-1); }
  unsigned short portu16 = (unsigned short)port; 
  addr.sin_port = htons(portu16);
  r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
  if (r < 0) { go_BYE(-1); } 
  r = listen(nfd, 10240);
  if (r < 0) { go_BYE(-1); } 
  int flags;
  if ( ((flags = fcntl(nfd, F_GETFL, 0)) < 0 ) 
      || ( fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0) ) { 
    go_BYE(-1); 
  }
  // TODO Bind to 0.0.0.0 or 127.0.0.1
  *ptr_nfd = nfd;
BYE:
  return status;
}

__attribute__((noreturn))
void *
webserver(
    void *arg
    )
{
  int status;
  int nfd;
  struct event_base **bases = NULL;
  struct evhttp **httpds = NULL;

  pthread_t *threads = NULL; // [n_threads]
  int n_threads;
  web_info_t *web_info = (web_info_t *)arg;

  if ( arg == NULL ) { go_BYE(-1); }
  n_threads = web_info->n_threads;
  if ( n_threads < 1 ) { go_BYE(-1); }
  uint32_t nT = (uint32_t)n_threads;

  threads = malloc(nT * sizeof(pthread_t));
  memset(threads, 0,  nT * sizeof(pthread_t));

  bases = malloc(nT * sizeof(struct event_base *));
  memset(bases, 0,  nT * sizeof(struct event_base *));
  web_info->bases = bases;

  httpds = malloc(nT * sizeof(struct evhttp *));
  memset(httpds, 0,  nT * sizeof(struct evhttp *));

  status = evthread_use_pthreads(); cBYE(status);
  int port = web_info->port; 
  if ( ( port <= 0 )  || ( port > 65535 ) ) { go_BYE(-1); } 

  status = bind_socket(port, &nfd); cBYE(status);

  for ( int i = 0; i < n_threads; i++ ) {
    bases[i] = event_init();
    if ( bases[i] == NULL ) { go_BYE(-1); }
    httpds[i]  = evhttp_new(bases[i]);
    if ( httpds[i] == NULL ) { go_BYE(-1); }

    //---------------------------------------------
    uint32_t sz = web_info->max_headers_size;
    if ( sz == 0 ) { sz = 4096; } 
    evhttp_set_max_headers_size(httpds[i], sz); 
    //---------------------------------------------
    sz = web_info->max_body_size;
    if ( sz == 0 ) { sz = 1048576; } 
    evhttp_set_max_body_size(httpds[i], sz); 
    //---------------------------------------------

    status = evhttp_accept_socket(httpds[i], nfd); cBYE(status);
    evhttp_set_gencb(httpds[i], handler, web_info);
    status = pthread_create(&(threads[i]), NULL, dispatch, bases[i]); 
    cBYE(status);
    printf("Forked thread %d \n", i);
  }
  for (int i = 0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }

BYE:
  if ( httpds != NULL ) { 
    for ( int i = 0; i < n_threads; i++ ) {
      if ( httpds[i] != NULL ) { 
        evhttp_free(httpds[i]);
      }
    }
  }
  if ( bases != NULL ) { 
    for ( int i = 0; i < n_threads; i++ ) {
      if ( bases[i] != NULL ) { 
      event_base_free(bases[i]);
      }
    }
  }
  free_if_non_null(threads);
  free_if_non_null(bases);
  free_if_non_null(httpds);
  libevent_global_shutdown();
  fprintf(stdout, "Web Server terminating\n");
  pthread_exit(NULL);
}
