/*	$OpenBSD: grep.h,v 1.24 2015/12/14 20:02:07 mmcc Exp $	*/

/*-
 * Copyright (c) 1999 James Howard and Dag-Erling Co�dan Sm�rgrav
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>

#include <limits.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef NOZ
#include <zlib.h>
#endif

#include "restrict_process.h"

#define VER_MAJ 0
#define VER_MIN 9

#define BIN_FILE_BIN	0
#define BIN_FILE_SKIP	1
#define BIN_FILE_TEXT	2

typedef struct {
	size_t		 len;
	long long	 line_no;
	off_t		 off;
	char		*file;
	char		*dat;
} str_t;

typedef struct {
	unsigned char	*pattern;
	size_t	 patternLen;
	int		 qsBc[UCHAR_MAX + 1];
	/* flags */
	int		 bol;
	int		 eol;
	int		 wmatch;
	int		 reversedSearch;
} fastgrep_t;

/* Flags passed to regcomp() and regexec() */
extern int	 cflags, eflags;

/* Command line flags */
extern int	 Aflag, Bflag, Eflag, Fflag, Hflag, Lflag,
		 Rflag, Zflag,
		 bflag, cflag, hflag, iflag, lflag, nflag, oflag, qflag, sflag,
		 vflag, wflag, xflag;
extern int	 binbehave;
extern const char *labelname;
extern FILE *stream_match, *stream_nomatch;

extern int	 first, matchall, tail, file_err;
extern size_t	 patterns;
extern char    **pattern;
extern fastgrep_t *fg_pattern;
extern regex_t	*r_pattern;

/* For regex errors  */
#define RE_ERROR_BUF 512
extern char	 re_error[RE_ERROR_BUF + 1];	/* Seems big enough */

/* util.c */
int		 procfile(char *fn);
int		 grep_tree(char **argv);
void		*grep_malloc(size_t size);
void		*grep_calloc(size_t nmemb, size_t size);
void		*grep_realloc(void *ptr, size_t size);
void		*grep_reallocarray(void *ptr, size_t nmemb, size_t size);
void		 printline(FILE *stream, str_t *line, int sep, regmatch_t *pmatch);
int		 fastcomp(fastgrep_t *, const char *);
void		 fgrepcomp(fastgrep_t *, const unsigned char *);

/* queue.c */
void		 initqueue(void);
void		 enqueue(str_t *x);
void		 printqueue(void);
void		 clearqueue(void);

/* mmfile.c */
typedef struct mmfile {
	int	 fd;
	size_t	 len;
	char	*base, *end, *ptr;
} mmf_t;

mmf_t		*mmopen(char *fn, char *mode);
void		 mmclose(mmf_t *mmf);
char		*mmfgetln(mmf_t *mmf, size_t *l);

/* file.c */
struct file;
typedef struct file file_t;

file_t		*grep_fdopen(int fd, char *mode);
file_t		*grep_open(char *path, char *mode);
int		 grep_bin_file(file_t *f);
char		*grep_fgetln(file_t *f, size_t *l);
void		 grep_close(file_t *f);

/* binary.c */
int		 bin_file(FILE * f);
#ifndef NOZ
int		 gzbin_file(gzFile * f);
#endif
int		 mmbin_file(mmf_t *f);

#ifndef HAVE_STRTONUM
long long strtonum(const char *numstr, long long minval, long long maxval,
        const char **errstrp);
#endif

#ifndef HAVE_REALLOCARRAY
void *reallocarray(void *optr, size_t nmemb, size_t size);
#endif

#ifndef HAVE_FGETLN
char *fgetln(FILE *fp, size_t *len);
#endif
