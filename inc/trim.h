#ifndef __trim_H
#define __trim_H
extern int 
trim(
    char * restrict inbuf,  /* input */
    char * restrict outbuf, 
    int n /* number of bytes allocated */
    );
#endif
