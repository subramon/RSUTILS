#include "q_incs.h"
#include <evhttp.h>
#include <pthread.h>
extern void
phandler(
    struct evhttp_request *req,
    void *arg
    );
extern void *
loop_handler(
    void *
    );
#include "web_struct.h"
#include "webserver.h"

// globals 
void *
webserver(
    void *arg
    )
{
  int status;
  if ( arg == NULL ) { go_BYE(-1); }
  web_info_t *web_info = (web_info_t *)arg;
  struct evhttp *httpd = NULL;
  struct event_base *base = NULL;
  uint32_t n_workers = web_info->n_workers;
  pthread_t *workers  = NULL;
  pthread_cond_t *condc = NULL; 
  pthread_cond_t *condp = NULL; 
  pthread_mutex_t *mutex = NULL; 
  // for signalling

  workers = malloc(n_workers * sizeof(pthread_t));
  memset(workers, 0, n_workers * sizeof(pthread_t));

  condc = malloc(n_workers * sizeof(pthread_cond_t));
  memset(workers, 0, n_workers * sizeof(pthread_cond_t));

  condp = malloc(n_workers * sizeof(pthread_cond_t));
  memset(workers, 0, n_workers * sizeof(pthread_cond_t));

  mutex = malloc(n_workers * sizeof(pthread_mutex_t));
  memset(workers, 0, n_workers * sizeof(pthread_mutex_t));

  for ( int i = 0; i < n_workers; i++ ) { 
    pthread_mutex_init(&(mutex[i]), NULL);
    pthread_cond_init(&(condc[i]), NULL);
    pthread_cond_init(&(condp[i]), NULL);
  }

  int port = web_info->port; 
  if ( ( port <= 0 )  || ( port > 65535 ) ) { go_BYE(-1); } 
  uint16_t portu16 = (uint16_t)(port);

  base = event_base_new();
  httpd = evhttp_new(base);
  //---------------------------------------------
  uint32_t sz = web_info->max_headers_size;
  if ( sz == 0 ) { sz = 4096; } 
  evhttp_set_max_headers_size(httpd, sz); 
  //---------------------------------------------
  sz = web_info->max_body_size;
  if ( sz == 0 ) { sz = 1048576; } 
  evhttp_set_max_body_size(httpd, sz); 
  //---------------------------------------------
  /* 
   *  Services will only receive packets from interfaces they listen to. 
   *  You can commonly specify 0.0.0.0 as listen address in the service, 
   *  to make it listen on all interfaces.
   *  https://askubuntu.com/questions/1136377/why-is-port-not-open-not-ufw-service-is-running
   *  */
  if ( web_info->is_external ) { 
    status = evhttp_bind_socket(httpd, "0.0.0.0", portu16);
  }
  else {
    status = evhttp_bind_socket(httpd, "127.0.0.1", portu16);
  }
  if ( status < 0 ) {
    fprintf(stderr, "portu16 %u busy \n", portu16); go_BYE(-1);
  }
  // Launch workers 
  /* TODO 
  for ( int i = 0; i < n_workers; i++ ) { 
    pthread_create(&(workers[i]), NULL, web_worker, web_info); 
  }
  */
  //---------------------
  web_info->workers = workers; 
  web_info->n_workers = n_workers; 
  web_info->base = base;
  evhttp_set_gencb(httpd, phandler, web_info); 
  event_base_dispatch(base);
  evhttp_free(httpd);
  event_base_free(base);

  for ( int i = 0; i < n_workers; i++ ) { 
    // TODO pthread_join(workers[i], NULL);
    pthread_mutex_destroy(&(mutex[i]));
    pthread_cond_destroy(&(condc[i]));
    pthread_cond_destroy(&(condp[i]));
  }

BYE:
  fprintf(stdout, "Web Server terminating\n");
  free_if_non_null(workers);
  pthread_exit(NULL);
}
