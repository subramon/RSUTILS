#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
//START_FOR_CDEF
extern int
read_csv(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    char ** const qtypes,
    char ** const out, // [ncols][nrows]
    const uint32_t * const width, // [ncols] width of a column (needed for SC)
    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    );
//STOP_FOR_CDEF
