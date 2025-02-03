#include <evhttp.h>
#ifndef __ALT_HANDLER_H
#define __ALT_HANDLER_H

#define MAX_LEN_API  31
#define MAX_LEN_ARGS 1023
#define MAX_LEN_OUTPUT 1048575
#define MAX_LEN_ERROR 1023
extern void *
loop_handler(
    void *arg
    );
typedef struct _loop_arg_t {
    struct evhttp_request *req;
    void *arg;
} loop_arg_t;
#endif // __ALT_HANDLER_H
