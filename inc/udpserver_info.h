#ifndef _UDPSERVER_H
#define _UDPSERVER_H

typedef struct _udpserver_info_t {
  int port; 
  // START: For debugging 
  int status;
  uint64_t num_pkts_received;
  uint64_t t_last_pkt;
  uint64_t t_first_pkt;
  uint64_t t_start;
  uint64_t t_stop;
  int thread_id;
  uint64_t utime;
  uint64_t stime;
  // STOP : For debugging 
} udpserver_info_t;
#endif // _UDPSERVER_H
