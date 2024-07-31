extern int
split_str(
    const char * const in_str,
    const char * const sep,
    char ***ptr_outs, // [n_outs]
    uint32_t *ptr_n_outs
    );
extern int
split_str_nullc_sep(
    char * const in_str, // INPUT 
    uint32_t len_in_str, // INPUT optional
    uint32_t n_outs, // INPUT 
    char ***ptr_outs // [n_outs] OUTPUT 
    );
