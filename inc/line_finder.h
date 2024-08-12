extern int
line_finder(
    const char * const X, // [nX] input 
    uint64_t nX,
    int fld_sep,
    int rec_sep,
    uint32_t ncol, // number of columns 
    uint32_t *max_width, // [ncol] can be null 
    uint32_t *ptr_eoln_idx
    );
