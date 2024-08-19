#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
extern int
read_csv(
    const char * const infile,
    const char *in_X,
    size_t in_nX,

    char ** const qtypes,
    void ** const out, // [ncols][nrows]
    const uint32_t * const width, // [ncols] width of a column (needed for SC)
    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    );
