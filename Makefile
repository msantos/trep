#	$OpenBSD: Makefile,v 1.7 2016/03/30 06:38:46 jmc Exp $

.PHONY: all clean test doc

PROG=	trep
SRCS=	binary.c file.c grep.c mmfile.c queue.c util.c \
		strtonum.c reallocarray.c fgetln.c \
		sandbox_null.c \
		sandbox_pledge.c \
		sandbox_seccomp.c \
		sandbox_capsicum.c \
		sandbox_rlimit.c

UNAME_SYS := $(shell uname -s)
ifeq ($(UNAME_SYS), Linux)
	CFLAGS ?= -D_FORTIFY_SOURCE=2 -O2 -fstack-protector-strong \
						-Wformat -Werror=format-security -fno-strict-aliasing
	TREP_SANDBOX ?= seccomp
else ifeq ($(UNAME_SYS), OpenBSD)
	CFLAGS ?= -DHAVE_STRTONUM -DHAVE_REALLOCARRAY -DHAVE_FGETLN \
						-D_FORTIFY_SOURCE=2 -O2 -fstack-protector-strong \
						-Wformat -Werror=format-security -fno-strict-aliasing
	TREP_SANDBOX ?= pledge
else ifeq ($(UNAME_SYS), FreeBSD)
	CFLAGS ?= -DHAVE_STRTONUM -DHAVE_REALLOCARRAY -DHAVE_FGETLN \
						-D_FORTIFY_SOURCE=2 -O2 -fstack-protector-strong \
	TREP_SANDBOX ?= capsicum
endif

TREP_SANDBOX ?= rlimit

TREP_CFLAGS ?= -g -Wall -fwrapv
CFLAGS += $(TREP_CFLAGS) \
		  -DTREP_SANDBOX=\"$(TREP_SANDBOX)\" -DTREP_SANDBOX_$(TREP_SANDBOX)

LDFLAGS += $(TREP_LDFLAGS) -Wl,-z,relro,-z,now

all: $(PROG)

$(PROG):
	$(CC) -DNOZ $(CFLAGS) -o $(PROG) $(SRCS) $(LDFLAGS)

clean:
	-@$(RM) trep

test: $(PROG)
	@PATH=.:$(PATH) bats test

doc:
	groff -mandoc -Thtml trep.1 > trep.1.html
	pandoc -t markdown_github -o README.md trep.1.html
