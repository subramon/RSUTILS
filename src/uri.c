/* ==========================================================================
* uri.c - URI parser.
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
#if URI_DEBUG
#include <stdio.h>
#endif

#include <string.h> /* memchr(3) */

#include <limits.h> /* CHAR_BIT */

#include <ctype.h> /* isalnum(3) */

#include <assert.h> /* assert(3) */

#include <sys/param.h> /* isset howmany MIN */

#include <sys/types.h>
#include <sys/uio.h> /* struct iovec */

#include "uri.h"


const struct uri uri_initializer = {
&uri_initializer, { 0, 0 },
}; /* uri_initializer */


const struct uri uri_http_initializer = {
&uri_http_initializer,

{ "http", sizeof "http" - 1 },
{ 0, 0 },
{ "/", sizeof "/" - 1 },
{ 0, 0 },
{ 0, 0 },
}; /* uri_http_initializer */


const struct uri_authority uri_authority_initializer = {
&uri_authority_initializer,

{ 0, 0 },
}; /* uri_authority_initializer */


const struct uri_query uri_query_initializer = {
&uri_query_initializer,

{ 0, 0 },
}; /* uri_query_initializer */


static const unsigned char *uri_scheme_next_invalid(const unsigned char *pos, const unsigned char *end) {
/* RFC 3986 Sec 3.1. */
while (pos < end && (isalnum(*pos) || *pos == '+' || *pos == '-' || *pos == '.'))
pos++;

return pos;
} /* uri_scheme_next_invalid() */


static const unsigned char *uri_authority_next_invalid(const unsigned char *pos, const unsigned char *end) {
/* RFC 3986 Sec 3.2. */
while (pos < end && *pos != '/' && *pos != '?' && *pos != '#')
pos++;

return pos;
} /* uri_authority_next_invalid() */


static const unsigned char *uri_path_next_invalid(const unsigned char *pos, const unsigned char *end) {
/* RFC 3986 Sec 3.3. */
while (pos < end && *pos != '?' && *pos != '#')
pos++;

return pos;
} /* uri_path_next_invalid() */


static const unsigned char *uri_query_next_invalid(const unsigned char *pos, const unsigned char *end) {
/* RFC 3986 Sec 3.4. */
while (pos < end && *pos != '#')
pos++;

return pos;
} /* uri_query_next_invalid() */


static const unsigned char *uri_fragment_next_invalid(const unsigned char *pos, const unsigned char *end) {
/* RFC 3986 Sec 3.5. */
while (pos < end)
pos++;

return pos;
} /* uri_fragment_next_invalid() */


enum uri_errno uri_parse(struct uri *uri, const void *str, size_t slen, int flags) {
struct { const unsigned char *pos, *nxt, *end; } src;

src.pos = str;
src.end = src.pos + slen;

/* Find scheme. */
src.nxt = uri_scheme_next_invalid(src.pos, src.end);

if (src.nxt < src.end && *src.nxt == ':' && src.nxt != src.pos) {
uri->scheme.iov_base = (void *)src.pos;
uri->scheme.iov_len = src.nxt - src.pos;

src.pos = src.nxt + 1; /* Skip ':' */
}

if (src.pos >= src.end)
return 0;

/* Find authority. */
if (&src.pos[1] < src.end && src.pos[0] == '/' && src.pos[1] == '/') {
src.pos += 2;

src.nxt = uri_authority_next_invalid(src.pos, src.end);

uri->authority.iov_base = (void *)src.pos;
uri->authority.iov_len = src.nxt - src.pos;

src.pos = src.nxt;
}

/* Find path. */
src.nxt = uri_path_next_invalid(src.pos, src.end);

if (src.nxt != src.pos) {
uri->path.iov_base = (void *)src.pos;
uri->path.iov_len = src.nxt - src.pos;

src.pos = src.nxt;
}

/* Find query. */
if (src.pos < src.end && *src.pos == '?') {
src.pos++;

src.nxt = uri_query_next_invalid(src.pos, src.end);

uri->query.iov_base = (void *)src.pos;
uri->query.iov_len = src.nxt - src.pos;

src.pos = src.nxt;
}

/* Find fragment. */
if (src.pos < src.end && *src.pos == '#') {
src.pos++;

src.nxt = uri_fragment_next_invalid(src.pos, src.end);

uri->fragment.iov_base = (void *)src.pos;
uri->fragment.iov_len = src.nxt - src.pos;

src.pos = src.nxt;
}

return 0;
} /* uri_parse() */


enum uri_errno uri_parse_authority(struct uri_authority *auth, const void *str, size_t slen, int flags) {
struct { const unsigned char *pos, *nxt, *end; } src;

src.pos = str;
src.end = src.pos + slen;

/* Find the end of either the userinfo or host:port. */
for (src.nxt = src.pos; src.nxt < src.end && *src.nxt != '@'; src.nxt++)
;;

if (src.nxt < src.end) {
auth->userinfo.iov_base = (void *)src.pos;
auth->userinfo.iov_len = src.nxt - src.pos;

src.pos = src.nxt + 1; /* Skip '@' */

if (src.pos >= src.end)
return 0; /* Nothing left. */
}

/* Find any port delimiter. */
for (src.nxt = src.end - 1; src.nxt >= src.pos && *src.nxt != ':'; src.nxt--)
;;

if (src.nxt >= src.pos) {
auth->port.iov_base = (void *)src.nxt + 1;
auth->port.iov_len = src.end - src.nxt;
} else
src.nxt = src.end;

auth->host.iov_base = (void *)src.pos;
auth->host.iov_len = src.nxt - src.pos;

return 0;
} /* uri_parse_authority() */


enum uri_errno uri_next_query(struct uri_query *q, const void *str, size_t slen, unsigned long *state, unsigned flags) {
struct { const unsigned char *pos, *nxt, *end; } src;
struct iovec key = { 0, 0 };
struct iovec value = { 0, 0 };

src.pos = (unsigned char *)str + *state;
src.end = (unsigned char *)str + slen;

next:
if (src.pos >= src.end)
return URI_ENOMORE;

/* Find '=' or '&'. */
for (src.nxt = src.pos; src.nxt < src.end && *src.nxt != '=' && *src.nxt != '&'; src.nxt++)
;;

key.iov_base = (void *)src.pos;
key.iov_len = src.nxt - src.pos;
src.pos = src.nxt + 1; /* Skip '=' or '&' */

if (src.nxt >= src.end || *src.nxt == '&')
goto done;

/* Find '&'. */
for (src.nxt = src.pos; src.nxt < src.end && *src.nxt != '&'; src.nxt++)
;;

value.iov_base = (void *)src.pos;
value.iov_len = src.nxt - src.pos;
src.pos = src.nxt + 1;

done:
*state = src.pos - (unsigned char *)str;

if ((key.iov_len == 0 && (flags & URI_SKIP_NO_KEY))
|| (value.iov_len == 0 && (flags & URI_SKIP_NO_VALUE)))
goto next;

if (key.iov_base != 0)
q->key = key;

if (value.iov_base != 0)
q->value = value;

return 0;
} /* uri_next_query() */


/*
* Invalid values have all bits set, the only value guaranteed not to occur
* in an "encoding" (if all bits are available it would be raw, not
* encoded).
*/
#define BASE16_VALUE_INVALID ((1U << CHAR_BIT) - 1)

static const unsigned char base16_value[1U << CHAR_BIT] = {
[0 ... (1U << CHAR_BIT) - 1] = BASE16_VALUE_INVALID,

/*
* We're assuming our compiler's locale's alnum range is mapped like
* ASCII (or is ASCII).
*/
['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4,
['5'] = 5, ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9,

['A'] = 10, ['B'] = 11, ['C'] = 12, ['D'] = 13,
['E'] = 14, ['F'] = 15,

['a'] = 10, ['b'] = 11, ['c'] = 12, ['d'] = 13,
['e'] = 14, ['f'] = 15,
}; /* base16_value */


#define URI_DECODE_ESCAPED (1UL << ((sizeof (uri_decode_t) * CHAR_BIT) - 1))

#define URI_DECODE_PUTC_NEEDMORE -1
#define URI_DECODE_PUTC_INVALID -2

static inline int uri_decode_putc(unsigned long *chars, int *nchars, unsigned char ch, uri_decode_t *state) {
unsigned char hi, lo;

if (!(*state & URI_DECODE_ESCAPED) && ch != '%')
return ch;

if ((*state & URI_DECODE_ESCAPED) && BASE16_VALUE_INVALID == base16_value[ch])
return URI_DECODE_PUTC_INVALID;

*chars = (*chars << CHAR_BIT) | (0xff & ch);

++(*nchars);

switch (*nchars) {
case 3:
hi = 0xff & (*chars >> (1 * CHAR_BIT));
lo = 0xff & (*chars >> (0 * CHAR_BIT));

*chars = 0;
*nchars = 0;
*state &= ~URI_DECODE_ESCAPED;

return (base16_value[hi] << 4) | base16_value[lo];
case 2:
assert(*state & URI_DECODE_ESCAPED);

return URI_DECODE_PUTC_NEEDMORE;
case 1:
*state |= URI_DECODE_ESCAPED;

return URI_DECODE_PUTC_NEEDMORE;
}

assert(0);

return URI_DECODE_PUTC_INVALID;
} /* uri_decode_putc() */


size_t uri_decode(void *buf, size_t blen, const void *str, size_t slen, char **end, const void *stop, size_t nstop, uri_decode_t *state) {
struct { unsigned char *pos, *end; } src, dst;
unsigned long chars = *state & ((1 << (3 * CHAR_BIT)) - 1);
int nchars = (*state >> (3 * CHAR_BIT)) & 0x03;
int ch;

src.pos = (unsigned char *)str;
src.end = (slen == -1)? (unsigned char *)-1 : src.pos + slen;

dst.pos = buf;
dst.end = buf + blen;

for (; src.pos < src.end && dst.pos < dst.end; src.pos++) {
if (nstop && memchr(stop, *src.pos, nstop))
break;

switch (ch = uri_decode_putc(&chars, &nchars, *src.pos, state)) {
case URI_DECODE_PUTC_INVALID:
goto finish;
case URI_DECODE_PUTC_NEEDMORE:
break;
default:
*(dst.pos++) = ch;
}
}

finish:
*state &= ~(0x03 << (3 * CHAR_BIT));
*state |= (nchars & 0x03) << (3 * CHAR_BIT);
*state |= chars & ((3 * CHAR_BIT) - 1);

*end = (char *)src.pos;

return dst.pos - (unsigned char *)buf;
} /* uri_decode() */


size_t uri_print_query(void *dbuf, size_t dlen, struct uri *u, const unsigned char *mask, size_t msiz, unsigned flags) {
struct { unsigned char *pos, *end; } dst
= { dbuf, (unsigned char *)dbuf + dlen };
struct uri_query q = uri_query_initializer;
unsigned long s = 0;
unsigned i = 0;
unsigned n = 0;

for (; 0 == uri_next_query(&q, u->query.iov_base, u->query.iov_len, &s, flags); i++, q = uri_query_initializer) {
if (msiz >= howmany(i + 1, CHAR_BIT) && isset(mask, i))
continue;

if (n++ > 0) {
if (dst.pos < dst.end)
*dst.pos = '&';

dst.pos++;
}

if (dst.pos < dst.end)
(void)memcpy(dst.pos, q.key.iov_base, MIN((size_t)(dst.end - dst.pos), q.key.iov_len));

dst.pos += q.key.iov_len;

if (URI_PART_ISDECLARED(&q, value)) {
if (dst.pos < dst.end)
*dst.pos = '=';

dst.pos++;

if (dst.pos < dst.end)
(void)memcpy(dst.pos, q.value.iov_base, MIN((size_t)(dst.end - dst.pos), q.value.iov_len));

dst.pos += q.value.iov_len;
}
}

return dst.pos - (unsigned char *)dbuf;
} /* uri_print_query() */


size_t uri_print(void *dbuf, size_t dlen, struct uri *u, const unsigned char *mask, size_t msiz, unsigned flags) {
struct { unsigned char *pos, *end; } dst
= { dbuf, (unsigned char *)dbuf + dlen };

if (!URI_PART_ISEMPTY(u, scheme)) {
if (dst.pos < dst.end)
(void)memcpy(dst.pos, u->scheme.iov_base, MIN((size_t)(dst.end - dst.pos), u->scheme.iov_len));

dst.pos += u->scheme.iov_len;

if (dst.pos < dst.end)
*dst.pos = ':';

dst.pos++;

if (dst.pos < dst.end)
*dst.pos = '/';

dst.pos++;

if (dst.pos < dst.end)
*dst.pos = '/';

dst.pos++;
}

if (!URI_PART_ISEMPTY(u, authority)) {
if (dst.pos < dst.end)
(void)memcpy(dst.pos, u->authority.iov_base, MIN((size_t)(dst.end - dst.pos), u->authority.iov_len));

dst.pos += u->authority.iov_len;
}

if (!URI_PART_ISEMPTY(u, path)) {
if (dst.pos < dst.end)
(void)memcpy(dst.pos, u->path.iov_base, MIN((size_t)(dst.end - dst.pos), u->path.iov_len));

dst.pos += u->path.iov_len;
}

if (URI_PART_ISDECLARED(u, query) || !URI_PART_ISEMPTY(u, query)) {
if (dst.pos < dst.end)
*dst.pos = '?';

dst.pos++;

dst.pos += uri_print_query(dst.pos, (dst.pos < dst.end)? dst.end - dst.pos : 0, u, mask, msiz, flags);
}

if (URI_PART_ISDECLARED(u, fragment) || !URI_PART_ISEMPTY(u, fragment)) {
if (dst.pos < dst.end)
*dst.pos = '#';

dst.pos++;

if (dst.pos < dst.end)
(void)memcpy(dst.pos, u->fragment.iov_base, MIN((size_t)(dst.end - dst.pos), u->fragment.iov_len));

dst.pos += u->fragment.iov_len;
}

return dst.pos - (unsigned char *)dbuf;
} /* uri_print() */


#ifdef URI_DEBUG

#include <stdio.h> /* printf(3) puts(3) */

#include <string.h> /* strlen(3) */


int main(int argc, char *argv[]) {
struct uri uri;
struct uri_query q;
int i, j, n;
unsigned long s;
char buf[256];
char dbuf[256];
unsigned char mask[16] = { 0 };
size_t dlen;

if (argc < 2)
return 0;

for (n = 0, i = 2; i < argc; i++) {
for (j = 0; argv[i][j] != '\0' && n < sizeof mask * CHAR_BIT; j++, n++) {
if ('1' == argv[i][j])
setbit(mask, n);
}
}

uri = uri_http_initializer;

uri_parse(&uri, argv[1], strlen(argv[1]), 0);

dlen = uri_print(dbuf, sizeof dbuf, &uri, mask, sizeof mask, 0);

printf("recomposed: %.*s\n", (int)MIN(dlen, sizeof dbuf), dbuf);

if (argc > 2)
return 0;

printf("%.*s[%s]\n", (i > 1), "\n", argv[i]);
printf("scheme: [%.*s]\n", (int)uri.scheme.iov_len, (char *)uri.scheme.iov_base);
printf("authority: [%.*s]\n", (int)uri.authority.iov_len, (char *)uri.authority.iov_base);
printf("path: [%.*s]\n", (int)uri.path.iov_len, (char *)uri.path.iov_base);

printf("query: [%.*s]", (int)uri.query.iov_len, (char *)uri.query.iov_base);

for (s = 0, q = uri_query_initializer; 0 == uri_next_query(&q, uri.query.iov_base, uri.query.iov_len, &s, URI_SKIP_NO_KEY); q = uri_query_initializer) {
uri_decode_t st = 0;
char *beg, *pos, *end;
size_t n, len = 0;

pos = beg
= q.value.iov_base;
end = pos + q.value.iov_len;

while (0 < (n = uri_decode(&buf[len], 1, pos, end - pos, &pos, 0, 0, &st))) {
len += n;
}

printf(" [%.*s=(%d)%.*s]", (int)q.key.iov_len, (char *)q.key.iov_base, (int)len, (int)len, buf);
}

puts("");

printf("fragment: [%.*s]\n", (int)uri.fragment.iov_len, (char *)uri.fragment.iov_base);

return 0;
} /* main() */

#endif /* URI_DEBUG */
