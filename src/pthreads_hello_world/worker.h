typedef struct _worker_results {
  int32_t i4;
  double f8;
  char *str;
} worker_results_t;

extern void* 
worker(
    void* arg
    );
