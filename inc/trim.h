#ifndef __trim_H
#define __trim_H
extern int 
trim(
    const char * const restrict inbuf,  /* input */
    char * const restrict outbuf, 
    int n /* number of bytes allocated */
    );
#endif
