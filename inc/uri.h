/* ==========================================================================
* uri.h - URI parser.
* --------------------------------------------------------------------------
* Copyright (c) 2007 William Ahern
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
* ==========================================================================
*/
#ifndef URI_H
#define URI_H

#include <sys/types.h>
#include <sys/uio.h> /* struct iovec */


#define URI_PART_ISDECLARED(u, m) \
((u)->m.iov_base != (u)->initializer->m.iov_base)

#define URI_PART_ISDEFINED(u, m) \
(URI_PART_ISDECLARED((u), m) && (u)->m.iov_len > 0)

#define URI_PART_ISEMPTY(u, m) \
((u)->m.iov_len == 0)


enum uri_errno {
URI_ESUCCESS = 0,
URI_ENOMORE,
URI_ESYNTAX,
}; /* enum uri_errno */


struct uri {
const struct uri *initializer;

struct iovec scheme;
struct iovec authority;
struct iovec path;
struct iovec query;
struct iovec fragment;
}; /* struct uri */

extern const struct uri uri_initializer;

extern const struct uri uri_http_initializer;


struct uri_authority {
const struct uri_authority *initializer;

struct iovec userinfo;
struct iovec host;
struct iovec port;
}; /* struct uri_authority */

extern const struct uri_authority uri_authority_initializer;


struct uri_query {
const struct uri_query *initializer;

struct iovec key;
struct iovec value;
}; /* struct uri_query */

extern const struct uri_query uri_query_initializer;


enum uri_errno uri_parse(struct uri *, const void *, size_t, int flags);

enum uri_errno uri_parse_authority(struct uri_authority *, const void *, size_t, int flags);


#define URI_SKIP_NO_KEY 1
#define URI_SKIP_NO_VALUE 2
#define URI_SKIP_EMPTY (URI_SKIP_NO_KEY | URI_SKIP_NO_VALUE)

enum uri_errno uri_next_query(struct uri_query *, const void *, size_t, unsigned long *, unsigned);


typedef unsigned long uri_decode_t;

size_t uri_decode(void *, size_t, const void *, size_t, char **, const void *, size_t, uri_decode_t *);


size_t uri_print_query(void *, size_t, struct uri *, const unsigned char *, size_t, unsigned);

size_t uri_print(void *, size_t, struct uri *, const unsigned char *, size_t, unsigned);


#endif /* URI_H */

