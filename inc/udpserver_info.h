#ifndef _UDPSERVER_H
#define _UDPSERVER_H

typedef int (*udp_proc_fn_t)( 
    const char *body,
    uint32_t n_body,
    void *W // information for custom processing
    );
typedef struct _udpserver_info_t {
  uint16_t port; 
  int halt; // to be set externally
  void *X; // anything else we want UDP server to have 
  udp_proc_fn_t proc_req_fn;
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
