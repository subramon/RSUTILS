extern int
num_lines_safe(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    uint32_t max_cell_width,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_rec_sep,
    uint32_t *ptr_nrows
    );
