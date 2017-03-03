#	$OpenBSD: Makefile,v 1.7 2016/03/30 06:38:46 jmc Exp $

PROG=	trep
SRCS=	binary.c file.c grep.c mmfile.c queue.c util.c
LINKS=	${BINDIR}/grep ${BINDIR}/egrep \
	${BINDIR}/grep ${BINDIR}/fgrep \
	${BINDIR}/grep ${BINDIR}/zgrep \
	${BINDIR}/grep ${BINDIR}/zegrep \
	${BINDIR}/grep ${BINDIR}/zfgrep \

CFLAGS+= -g -Wall

LDADD=	-lbsd
DPADD=	${LIBZ}

all:
	$(CC) -DNOZ $(CFLAGS) -o $(PROG) $(SRCS) $(LDADD)

clean:
	-@$(RM) trep
