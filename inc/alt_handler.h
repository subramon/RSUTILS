#include <evhttp.h>
#ifndef __ALT_HANDLER_H
#define __ALT_HANDLER_H

#define MAX_LEN_API  31
#define MAX_LEN_ARGS 1023
#define MAX_LEN_OUTPUT 1048575
#define MAX_LEN_ERROR 1023
extern void *
alt_handler(
    void *arg
    );
typedef struct _core_lhandler_arg_t {
    struct evhttp_request *req;
    void *arg;
} core_lhandler_arg_t;
#endif // __ALT_HANDLER_H
