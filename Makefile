#	$OpenBSD: Makefile,v 1.7 2016/03/30 06:38:46 jmc Exp $

PROG=	trep
SRCS=	binary.c file.c grep.c mmfile.c queue.c util.c \
		strtonum.c reallocarray.c
LINKS=	${BINDIR}/grep ${BINDIR}/egrep \
	${BINDIR}/grep ${BINDIR}/fgrep \
	${BINDIR}/grep ${BINDIR}/zgrep \
	${BINDIR}/grep ${BINDIR}/zegrep \
	${BINDIR}/grep ${BINDIR}/zfgrep \

UNAME_SYS := $(shell uname -s)
ifeq ($(UNAME_SYS), Linux)
	CFLAGS ?= -D_FORTIFY_SOURCE=2 -O2 -fstack-protector \
			  --param=ssp-buffer-size=4 -Wformat -Werror=format-security \
			  -fno-strict-aliasing
	LDADD=	-lbsd
else
	CFLAGS ?= -DHAVE_STRTONUM -DHAVE_REALLOCARRAY
endif

all:
	$(CC) -g -Wall -DNOZ $(CFLAGS) -o $(PROG) $(SRCS) $(LDADD)

clean:
	-@$(RM) trep
