#include "q_incs.h"
#include <evhttp.h>
#include <pthread.h>
extern void
handler(
    struct evhttp_request *req,
    void *arg
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
  int port = web_info->port; 
  struct evhttp *httpd = NULL;
  struct event_base *base = NULL;
  if ( ( port <= 0 )  || ( port > 65535 ) ) { go_BYE(-1); } 
  uint16_t portu16 = (uint16_t)(port);

  base = event_base_new();
  httpd = evhttp_new(base);
  evhttp_set_max_headers_size(httpd, 4096); // TODO P4 FIX 
  evhttp_set_max_body_size(httpd, 1048576); // TODO P4 FIX 

  /* 
   *  Services will only receive packets from interfaces they listen to. You can commonly specify 0.0.0.0 as listen address in the service, to make it listen on all interfaces.
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
  //---------------------
  web_info->base = base;
  evhttp_set_gencb(httpd, handler, web_info);
  event_base_dispatch(base);
  evhttp_free(httpd);
  event_base_free(base);

BYE:
  fprintf(stdout, "Web Server terminating\n");
  pthread_exit(NULL);
}
