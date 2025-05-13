#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "q_incs.h"
#include "q_macros.h"
#include "get_time_usec.h"
#include "udpserver_info.h"
#include "udpserver.h"


__attribute__((noreturn))
void *udpserver(
    void *arg
    )
{
  int status = 0;
  udpserver_info_t *udp_info = (udpserver_info_t *)arg;
  if ( udp_info == NULL ) { go_BYE(-1); }
  uint16_t port = udp_info->port; 

  int sockfd; /* socket */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[UDP_BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)  { 
    perror("ERROR opening socket"); go_BYE(-1); 
  }

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
      (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(port);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
        sizeof(serveraddr)) < 0)  {
    perror("ERROR on binding"); go_BYE(-1); 
  }

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  bool first = true; 
  udp_info->t_start = get_time_usec(); 
  while (1) {
    if ( first ) { 
      udp_info->t_first_pkt = get_time_usec(); 
      first = false;
    }
    int l_halt; __atomic_load(&(udp_info->halt), &l_halt, 0);
    if ( l_halt == 1 ) { break; }

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, UDP_BUFSIZE);
    ssize_t n_recv = recvfrom(sockfd, buf, UDP_BUFSIZE, 0,
        (struct sockaddr *) &clientaddr, &clientlen);
    if (n_recv < 0) { 
      perror("ERROR in recvfrom"); go_BYE(-1);
    }

    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL) { 
      perror("ERROR on gethostbyaddr"); go_BYE(-1); 
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) {
      perror("ERROR on inet_ntoa\n"); go_BYE(-1);
    }
    printf("server received datagram from %s (%s)\n", 
        hostp->h_name, hostaddrp);
    printf("server received %u bytes\n", (uint32_t)n_recv);
    // Now for the custom processing 
    status = udp_info->proc_req_fn(buf, (uint32_t)n_recv, udp_info->X); 
    cBYE(status);

    udp_info->t_last_pkt = get_time_usec(); 
    udp_info->num_pkts_received++;
  }
  udp_info->t_stop = get_time_usec(); 
BYE:
  udp_info->status = status; 
  fprintf(stdout, "UDP Server terminating\n");
  pthread_exit(NULL);
}
