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

extern void
handler(
    struct evhttp_request *req,
    void *arg
    );
extern void *
webserver(
    void *arg
    );
int 
BindSocket(
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
  if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
      || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
    return -1;
  return nfd;
}
int
serv(
    int port,
    int nthreads
    )
{
  int r;
  void *web_info = NULL;
  int nfd = BindSocket(port);
  if (nfd < 0) return -1;
  pthread_t ths[nthreads];
  for (int i = 0; i < nthreads; i++) {
    struct event_base *base = event_init();
    if (base == NULL) { go_BYE(-1); } 
    struct evhttp *httpd = evhttp_new(base);
    if (httpd == NULL) { go_BYE(-1); } 
    r = evhttp_accept_socket(httpd, nfd);
    if (r != 0) { go_BYE(-1); } 
    evhttp_set_gencb(httpd, handler, web_info);
    r = pthread_create(&ths[i], NULL, dispatch, base);
    if (r != 0) { go_BYE(-1); } 
  }
  for (int i = 0; i < nthreads; i++) {
    pthread_join(ths[i], NULL);
  }
}

