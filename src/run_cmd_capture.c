#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "run_cmd_capture.h"

/* ------------------------------------------------------------------ */
/* Helper: read an entire file into a malloc'd, NUL-terminated buffer. */
/* ------------------------------------------------------------------ */
static char *read_entire_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    size_t cap = 4096, len = 0;
    char *buf = malloc(cap);
    if (!buf) { fclose(f); return NULL; }

    for (;;) {
        if (len == cap) {
            cap *= 2;
            char *nb = realloc(buf, cap);
            if (!nb) { free(buf); fclose(f); return NULL; }
            buf = nb;
        }
        size_t n = fread(buf + len, 1, cap - len, f);
        len += n;
        if (n == 0) {
            if (feof(f)) break;
            if (ferror(f)) { free(buf); fclose(f); return NULL; }
        }
    }
    char *nb = realloc(buf, len + 1);
    if (nb) buf = nb;
    buf[len] = '\0';
    fclose(f);
    return buf;
}

/* ------------------------------------------------------------------ */
/* Run `cmd` via system(), capturing stdout, stderr and exit status.   */
/*                                                                     */
/* Returns:                                                            */
/*    0   success – *status_out, *str_out, *str_err are filled in      */
/*   -1   internal failure (see errno)                                 */
/*                                                                     */
/* *str_out / *str_err are malloc'd, NUL-terminated (possibly empty)   */
/* strings the caller must free().  *status_out is the child's exit    */
/* status (WEXITSTATUS), or 128+signal if it was killed by a signal.   */
/* ------------------------------------------------------------------ */
int 
run_cmd_capture(
    const char * const cmd,
    int        *status_out,
    char      **str_out,
    char      **str_err
    )
{
    if (!cmd || !status_out || !str_out || !str_err)
        return -1;

    *status_out = -1;
    *str_out    = NULL;
    *str_err    = NULL;

    /* Two unique temp files, one per stream. */
    char out_tmpl[] = "/tmp/cmdoutXXXXXX";
    char err_tmpl[] = "/tmp/cmderrXXXXXX";
    int fd_out = mkstemp(out_tmpl);
    int fd_err = mkstemp(err_tmpl);
    if (fd_out < 0 || fd_err < 0) {
        if (fd_out >= 0) { close(fd_out); unlink(out_tmpl); }
        if (fd_err >= 0) { close(fd_err); unlink(err_tmpl); }
        return -1;
    }
    close(fd_out);
    close(fd_err);

    /* Wrap the user command in a subshell so any trailing ';' or '&&' */
    /* in it is harmless, then redirect both streams to our temp files. */
    size_t need = strlen(cmd) + strlen(out_tmpl) + strlen(err_tmpl) + 32;
    char *full = malloc(need);
    if (!full) {
        unlink(out_tmpl); unlink(err_tmpl);
        return -1;
    }
    snprintf(full, need, "( %s ) > '%s' 2> '%s'", cmd, out_tmpl, err_tmpl);

    int rc = system(full);
    free(full);

    if (rc == -1) {               /* fork/exec of /bin/sh failed */
        unlink(out_tmpl); unlink(err_tmpl);
        return -1;
    }

    if (WIFEXITED(rc))
        *status_out = WEXITSTATUS(rc);
    else if (WIFSIGNALED(rc))
        *status_out = 128 + WTERMSIG(rc);
    else
        *status_out = rc;

    *str_out = read_entire_file(out_tmpl);
    *str_err = read_entire_file(err_tmpl);

    unlink(out_tmpl);
    unlink(err_tmpl);

    if (!*str_out) *str_out = strdup("");
    if (!*str_err) *str_err = strdup("");
    return (*str_out && *str_err) ? 0 : -1;
}
