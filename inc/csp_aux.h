extern int
csp_free(
    csp_info_t * ptr_csp_info
    );
extern int
csp_uncache(
    csp_info_t *csp_info, // [n_csp]
    char **csp_labels, // [n_csp]
    uint32_t n_csp,
    const char * const csp_label, 
    const char * const frag_label
    );
