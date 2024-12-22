#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
//START_FOR_CDEF
extern int
read_csv(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    const qtype_t * qtypes, // [ncols] 
    const uint32_t * const widths, // [ncols] (needed for SC)
    char ** const formats, // [ncols] (needed for TM)
    const bool * const is_load, // [ncols] whether col has nulls
    const bool * const has_nulls, // [ncols] whether col to be loadde

    char ** const out, // [ncols][nrows]
    bool ** const nn_out, // [ncols][nrows]

    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    );
//STOP_FOR_CDEF
