#include <evhttp.h>
#ifndef __HANDLER_H
#define __HANDLER_H

#define MAX_LEN_API  31
#define MAX_LEN_ARGS 1023
#define MAX_LEN_OUTPUT 1048575
#define MAX_LEN_ERROR 1023
extern void
handler(
    struct evhttp_request *req,
    void *arg
    );
#endif // __HANDLER_H
