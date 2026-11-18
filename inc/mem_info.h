#include <stddef.h>
#include <stdint.h>

extern int 
mem_info(
    size_t *ptr_pagesize, 
    int64_t *ptr_bytes, 
    size_t *ptr_pages
    );
