extern int 
send_udp_pkt(
    const char * const hostname, 
    uint16_t portno, 
    const char * const buf, 
    uint32_t buflen
    );
extern int 
send_udp_setup(
    const char * const hostname, 
    uint16_t portno,
    int *ptr_sockfd,
    struct sockaddr_in *ptr_serveraddr
    );
