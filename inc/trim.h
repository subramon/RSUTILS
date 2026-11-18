#ifndef RSUTILS_INC_TRIM_H
#define RSUTILS_INC_TRIM_H
extern int 
trim(
    const char * const restrict inbuf,  /* input */
    char * const restrict outbuf, 
    int n /* number of bytes allocated */
    );
#endif
