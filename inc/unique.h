extern int 
num_unique(
    const char * constvals,  // [n] 
    const char * const nn, // [n]
    uint32_t n, 
    qtype_t qtype, 
    uint32_t * restrict ptr_num
    );
extern bool
is_unique(
    const char * const vals,  // [n] 
    const char * const nn, // [n]  tells us whether value is null or not. 0=> null
    uint32_t n, 
    qtype_t qtype
    );
extern int 
num_unique_saturated(
    const char * const vals,  // [n] 
    uint32_t n, 
    qtype_t qtype,
    uint32_t max_n_uq,
    uint32_t * restrict ptr_n_uq
    );
