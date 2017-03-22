#	$OpenBSD: Makefile,v 1.7 2016/03/30 06:38:46 jmc Exp $

.PHONY: all clean test

PROG=	trep
SRCS=	binary.c file.c grep.c mmfile.c queue.c util.c \
		strtonum.c reallocarray.c fgetln.c \
		trep_sandbox_null.c \
		trep_sandbox_pledge.c \
		trep_sandbox_seccomp.c \
		trep_sandbox_capsicum.c \
		trep_sandbox_rlimit.c
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
	TREP_SANDBOX ?= TREP_SANDBOX_SECCOMP
else ifeq ($(UNAME_SYS), OpenBSD)
	TREP_SANDBOX ?= TREP_SANDBOX_PLEDGE
else ifeq ($(UNAME_SYS), FreeBSD)
	TREP_SANDBOX ?= TREP_SANDBOX_CAPSICUM
else
	CFLAGS ?= -DHAVE_STRTONUM -DHAVE_REALLOCARRAY -DHAVE_FGETLN
endif

TREP_SANDBOX ?= TREP_SANDBOX_RLIMIT

CFLAGS += -DTREP_SANDBOX=\"$(TREP_SANDBOX)\" -D$(TREP_SANDBOX)

all:
	$(CC) -g -Wall -DNOZ $(CFLAGS) -o $(PROG) $(SRCS)

clean:
	-@$(RM) trep

test:
	-@PATH=.:$(PATH) bats test
