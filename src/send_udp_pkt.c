/* 
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "q_incs.h"
#include "q_macros.h"
#include "send_udp_pkt.h"

#define BUFSIZE 1460

/* 
 * error - wrapper for perror
 */
static void 
error(
    const char * const msg
    ) 
{
  perror(msg);
  exit(0);
}

int 
send_udp_pkt(
    const char * const hostname, 
    uint16_t portno, 
    const char * const buf, 
    uint32_t buflen
    )
{
  int status = 0;
  int sockfd = -1; 
  int serverlen;
  struct sockaddr_in serveraddr;
  ssize_t n_send;

  status = send_udp_setup(hostname, portno, &sockfd, &serveraddr);
  cBYE(status);
  /* send the message to the server */
  serverlen = sizeof(serveraddr);
  n_send = sendto(sockfd, buf, buflen, 0, &serveraddr, serverlen);
  if (n_send < 0)  {
    error("ERROR in sendto"); go_BYE(-1);
  }
BYE:
  if ( sockfd >= 0 ) { close(sockfd); }
  return status;
}

int 
send_udp_setup(
    const char * const hostname, 
    uint16_t portno,
    int *ptr_sockfd,
    struct sockaddr_in *ptr_serveraddr
    )
{
  int status = 0;
  int sockfd = -1; 
  struct hostent *server;
  struct sockaddr_in serveraddr;

  /* socket: create the socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)  { 
    error("ERROR opening socket"); go_BYE(-1);
  }

  /* gethostbyname: get the server's DNS entry */
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host as %s\n", hostname); go_BYE(-1); 
  }

  /* build the server's Internet address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
      (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);

  *ptr_serveraddr = serveraddr;
  *ptr_sockfd = sockfd;
BYE:
  return status;
}
