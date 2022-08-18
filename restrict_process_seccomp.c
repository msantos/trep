/* Copyright (c) 2017-2022, Michael Santos <michael.santos@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifdef RESTRICT_PROCESS_seccomp
#include <errno.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <stddef.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

/* macros from openssh-7.2/restrict_process-seccomp-filter.c */

/* Linux seccomp_filter restrict_process */
#define SECCOMP_FILTER_FAIL SECCOMP_RET_KILL

/* Use a signal handler to emit violations when debugging */
#ifdef SANDBOX_SECCOMP_FILTER_DEBUG
#undef SECCOMP_FILTER_FAIL
#define SECCOMP_FILTER_FAIL SECCOMP_RET_TRAP
#endif /* SANDBOX_SECCOMP_FILTER_DEBUG */

/* Simple helpers to avoid manual errors (but larger BPF programs). */
#define SC_DENY(_nr, _errno)                                                   \
  BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, __NR_##_nr, 0, 1),                       \
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ERRNO | (_errno))
#define SC_ALLOW(_nr)                                                          \
  BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, __NR_##_nr, 0, 1),                       \
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW)
#define SC_ALLOW_ARG(_nr, _arg_nr, _arg_val)                                   \
  BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, __NR_##_nr, 0,                           \
           4), /* load first syscall argument */                               \
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS,                                       \
               offsetof(struct seccomp_data, args[(_arg_nr)])),                \
      BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, (_arg_val), 0, 1),                   \
      BPF_STMT(BPF_RET + BPF_K,                                                \
               SECCOMP_RET_ALLOW), /* reload syscall number; all rules expect  \
                                      it in accumulator */                     \
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, nr))

/*
 * http://outflux.net/teach-seccomp/
 * https://github.com/gebi/teach-seccomp
 *
 */
#define syscall_nr (offsetof(struct seccomp_data, nr))
#define arch_nr (offsetof(struct seccomp_data, arch))

#if defined(__i386__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_I386
#elif defined(__x86_64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_X86_64
#elif defined(__arm__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_ARM
#elif defined(__aarch64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_AARCH64
#else
#warning "seccomp: unsupported platform"
#define SECCOMP_AUDIT_ARCH 0
#endif

int restrict_process_init() {
  struct sock_filter filter[] = {
      /* Ensure the syscall arch convention is as expected. */
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, arch)),
      BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SECCOMP_AUDIT_ARCH, 1, 0),
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_FILTER_FAIL),
      /* Load the syscall number for checking. */
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, nr)),

  /* Syscalls to non-fatally deny */

  /* Syscalls to allow */
#ifdef __NR_prctl
      SC_ALLOW(prctl),
#endif
#ifdef __NR_brk
      SC_ALLOW(brk),
#endif
#ifdef __NR_clock_getres
      SC_ALLOW(clock_getres),
#endif
#ifdef __NR_clock_gettime
      SC_ALLOW(clock_gettime),
#endif
#ifdef __NR_clock_gettime64
      SC_ALLOW(clock_gettime64),
#endif
#ifdef __NR_close
      SC_ALLOW(close),
#endif
#ifdef __NR_dup
      SC_ALLOW(dup),
#endif
#ifdef __NR_dup2
      SC_ALLOW(dup2),
#endif
#ifdef __NR_dup3
      SC_ALLOW(dup3),
#endif
#ifdef __NR_exit_group
      SC_ALLOW(exit_group),
#endif
#ifdef __NR_fchdir
      SC_ALLOW(fchdir),
#endif
#ifdef __NR_fcntl
      SC_ALLOW(fcntl),
#endif
#ifdef __NR_fcntl64
      SC_ALLOW(fcntl64),
#endif
#ifdef __NR_fstat
      SC_ALLOW(fstat),
#endif
#ifdef __NR_fstat64
      SC_ALLOW(fstat64),
#endif
#ifdef __NR_fsync
      SC_ALLOW(fsync),
#endif
#ifdef __NR_ftruncate
      SC_ALLOW(ftruncate),
#endif
#ifdef __NR_getdents
      SC_ALLOW(getdents),
#endif
#ifdef __NR_getdtablecount
      SC_ALLOW(getdtablecount),
#endif
#ifdef __NR_getegid
      SC_ALLOW(getegid),
#endif
#ifdef __NR_getentropy
      SC_ALLOW(getentropy),
#endif
#ifdef __NR_geteuid
      SC_ALLOW(geteuid),
#endif
#ifdef __NR_getgid
      SC_ALLOW(getgid),
#endif
#ifdef __NR_getgroups
      SC_ALLOW(getgroups),
#endif
#ifdef __NR_getitimer
      SC_ALLOW(getitimer),
#endif
#ifdef __NR_getlogin
      SC_ALLOW(getlogin),
#endif
#ifdef __NR_getpgid
      SC_ALLOW(getpgid),
#endif
#ifdef __NR_getpgrp
      SC_ALLOW(getpgrp),
#endif
#ifdef __NR_getpid
      SC_ALLOW(getpid),
#endif
#ifdef __NR_getppid
      SC_ALLOW(getppid),
#endif
#ifdef __NR_getresgid
      SC_ALLOW(getresgid),
#endif
#ifdef __NR_getresuid
      SC_ALLOW(getresuid),
#endif
#ifdef __NR_getrlimit
      SC_ALLOW(getrlimit),
#endif
#ifdef __NR_getsid
      SC_ALLOW(getsid),
#endif
#ifdef __NR_getthrid
      SC_ALLOW(getthrid),
#endif
#ifdef __NR_gettimeofday
      SC_ALLOW(gettimeofday),
#endif
#ifdef __NR_getuid
      SC_ALLOW(getuid),
#endif
#ifdef __NR_getuid
      SC_ALLOW(getuid),
#endif
#ifdef __NR_ioctl
      SC_ALLOW(ioctl),
#endif
#ifdef __NR_issetugid
      SC_ALLOW(issetugid),
#endif
#ifdef __NR_lseek
      SC_ALLOW(lseek),
#endif
#ifdef __NR__llseek
      SC_ALLOW(_llseek),
#endif
#ifdef __NR_lstat
      SC_ALLOW(lstat),
#endif
#ifdef __NR_lstat64
      SC_ALLOW(lstat64),
#endif
#ifdef __NR_madvise
      SC_ALLOW(madvise),
#endif
#ifdef __NR_minherit
      SC_ALLOW(minherit),
#endif
#ifdef __NR_mmap
      SC_ALLOW(mmap),
#endif
#ifdef __NR_mmap2
      SC_ALLOW(mmap2),
#endif
#ifdef __NR_mprotect
      SC_ALLOW(mprotect),
#endif
#ifdef __NR_mquery
      SC_ALLOW(mquery),
#endif
#ifdef __NR_mremap
      SC_ALLOW(mremap),
#endif
#ifdef __NR_munmap
      SC_ALLOW(munmap),
#endif
#ifdef __NR_nanosleep
      SC_ALLOW(nanosleep),
#endif
#ifdef __NR_pipe
      SC_ALLOW(pipe),
#endif
#ifdef __NR_pipe2
      SC_ALLOW(pipe2),
#endif
#ifdef __NR_poll
      SC_ALLOW(poll),
#endif
#ifdef __NR_pread
      SC_ALLOW(pread),
#endif
#ifdef __NR_preadv
      SC_ALLOW(preadv),
#endif
#ifdef __NR_pwrite
      SC_ALLOW(pwrite),
#endif
#ifdef __NR_pwritev
      SC_ALLOW(pwritev),
#endif
#ifdef __NR_read
      SC_ALLOW(read),
#endif
#ifdef __NR_readv
      SC_ALLOW(readv),
#endif
#ifdef __NR_recvfrom
      SC_ALLOW(recvfrom),
#endif
#ifdef __NR_recvmsg
      SC_ALLOW(recvmsg),
#endif
#ifdef __NR_select
      SC_ALLOW(select),
#endif
#ifdef __NR_sendmsg
      SC_ALLOW(sendmsg),
#endif
#ifdef __NR_sendto
      SC_ALLOW(sendto),
#endif
#ifdef __NR_setitimer
      SC_ALLOW(setitimer),
#endif
#ifdef __NR_shutdown
      SC_ALLOW(shutdown),
#endif
#ifdef __NR_sigaction
      SC_ALLOW(sigaction),
#endif
#ifdef __NR_sigprocmask
      SC_ALLOW(sigprocmask),
#endif
#ifdef __NR_sigreturn
      SC_ALLOW(sigreturn),
#endif
#ifdef __NR_socketpair
      SC_ALLOW(socketpair),
#endif
#ifdef __NR_umask
      SC_ALLOW(umask),
#endif
#ifdef __NR_wait4
      SC_ALLOW(wait4),
#endif
#ifdef __NR_write
      SC_ALLOW(write),
#endif
#ifdef __NR_writev
      SC_ALLOW(writev),
#endif
#ifdef __NR_chdir
      SC_ALLOW(chdir),
#endif
#ifdef __NR_getcwd
      SC_ALLOW(getcwd),
#endif
#ifdef __NR_openat
      SC_ALLOW(openat),
#endif
#ifdef __NR_open
      SC_ALLOW(open),
#endif
#ifdef __NR_fstatat
      SC_ALLOW(fstatat),
#endif
#ifdef __NR_faccessat
      SC_ALLOW(faccessat),
#endif
#ifdef __NR_readlinkat
      SC_ALLOW(readlinkat),
#endif
#ifdef __NR_lstat
      SC_ALLOW(lstat),
#endif
#ifdef __NR_chmod
      SC_ALLOW(chmod),
#endif
#ifdef __NR_fchmod
      SC_ALLOW(fchmod),
#endif
#ifdef __NR_fchmodat
      SC_ALLOW(fchmodat),
#endif
#ifdef __NR_chflags
      SC_ALLOW(chflags),
#endif
#ifdef __NR_chflagsat
      SC_ALLOW(chflagsat),
#endif
#ifdef __NR_chown
      SC_ALLOW(chown),
#endif
#ifdef __NR_fchown
      SC_ALLOW(fchown),
#endif
#ifdef __NR_fchownat
      SC_ALLOW(fchownat),
#endif
#ifdef __NR_getfsstat
      SC_ALLOW(getfsstat),
#endif
#ifdef __NR_restart_syscall
      SC_ALLOW(restart_syscall),
#endif

#ifdef __NR_newfstatat
      SC_ALLOW(newfstatat),
#endif
#ifdef __NR_getdents64
      SC_ALLOW(getdents64),
#endif
#ifdef __NR_getrandom
      SC_ALLOW(getrandom),
#endif

      /* Default deny */
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_FILTER_FAIL)};

  struct sock_fprog prog = {
      .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
      .filter = filter,
  };

  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0)
    return -1;

  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog))
    return -1;

  return 0;
}

int restrict_process_stdin() {
  struct sock_filter filter[] = {
      /* Ensure the syscall arch convention is as expected. */
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, arch)),
      BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SECCOMP_AUDIT_ARCH, 1, 0),
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_FILTER_FAIL),
      /* Load the syscall number for checking. */
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, nr)),

  /* Syscalls to non-fatally deny */

  /* Syscalls to allow */
#ifdef __NR_ioctl
      SC_ALLOW(ioctl),
#endif
#ifdef __NR__llseek
      SC_ALLOW(_llseek),
#endif

  /* Used to test if input is file or stdin */
#ifdef __NR_fcntl
      SC_ALLOW(fcntl),
#endif
#ifdef __NR_fcntl64
      SC_ALLOW(fcntl64),
#endif
#ifdef __NR_fstat
      SC_ALLOW(fstat),
#endif
#ifdef __NR_fstat64
      SC_ALLOW(fstat64),
#endif

#ifdef __NR_brk
      SC_ALLOW(brk),
#endif
#ifdef __NR_clock_getres
      SC_ALLOW(clock_getres),
#endif
#ifdef __NR_clock_gettime64
      SC_ALLOW(clock_gettime64),
#endif
#ifdef __NR_close
      SC_ALLOW(close),
#endif
#ifdef __NR_dup
      SC_ALLOW(dup),
#endif
#ifdef __NR_dup2
      SC_ALLOW(dup2),
#endif
#ifdef __NR_dup3
      SC_ALLOW(dup3),
#endif
#ifdef __NR_exit_group
      SC_ALLOW(exit_group),
#endif
#ifdef __NR_fchdir
      SC_ALLOW(fchdir),
#endif
#ifdef __NR_fsync
      SC_ALLOW(fsync),
#endif
#ifdef __NR_ftruncate
      SC_ALLOW(ftruncate),
#endif
#ifdef __NR_getdents
      SC_ALLOW(getdents),
#endif
#ifdef __NR_getdtablecount
      SC_ALLOW(getdtablecount),
#endif
#ifdef __NR_getegid
      SC_ALLOW(getegid),
#endif
#ifdef __NR_getentropy
      SC_ALLOW(getentropy),
#endif
#ifdef __NR_geteuid
      SC_ALLOW(geteuid),
#endif
#ifdef __NR_getgid
      SC_ALLOW(getgid),
#endif
#ifdef __NR_getgroups
      SC_ALLOW(getgroups),
#endif
#ifdef __NR_getitimer
      SC_ALLOW(getitimer),
#endif
#ifdef __NR_getlogin
      SC_ALLOW(getlogin),
#endif
#ifdef __NR_getpgid
      SC_ALLOW(getpgid),
#endif
#ifdef __NR_getpgrp
      SC_ALLOW(getpgrp),
#endif
#ifdef __NR_getpid
      SC_ALLOW(getpid),
#endif
#ifdef __NR_getppid
      SC_ALLOW(getppid),
#endif
#ifdef __NR_getresgid
      SC_ALLOW(getresgid),
#endif
#ifdef __NR_getresuid
      SC_ALLOW(getresuid),
#endif
#ifdef __NR_getrlimit
      SC_ALLOW(getrlimit),
#endif
#ifdef __NR_getsid
      SC_ALLOW(getsid),
#endif
#ifdef __NR_getthrid
      SC_ALLOW(getthrid),
#endif
#ifdef __NR_gettimeofday
      SC_ALLOW(gettimeofday),
#endif
#ifdef __NR_getuid
      SC_ALLOW(getuid),
#endif
#ifdef __NR_getuid
      SC_ALLOW(getuid),
#endif
#ifdef __NR_ioctl
      SC_ALLOW(ioctl),
#endif
#ifdef __NR_issetugid
      SC_ALLOW(issetugid),
#endif
#ifdef __NR_lseek
      SC_ALLOW(lseek),
#endif
#ifdef __NR__llseek
      SC_ALLOW(_llseek),
#endif
#ifdef __NR_madvise
      SC_ALLOW(madvise),
#endif
#ifdef __NR_minherit
      SC_ALLOW(minherit),
#endif
#ifdef __NR_mmap
      SC_ALLOW(mmap),
#endif
#ifdef __NR_mmap2
      SC_ALLOW(mmap2),
#endif
#ifdef __NR_mprotect
      SC_ALLOW(mprotect),
#endif
#ifdef __NR_mquery
      SC_ALLOW(mquery),
#endif
#ifdef __NR_mremap
      SC_ALLOW(mremap),
#endif
#ifdef __NR_munmap
      SC_ALLOW(munmap),
#endif
#ifdef __NR_nanosleep
      SC_ALLOW(nanosleep),
#endif
#ifdef __NR_pipe
      SC_ALLOW(pipe),
#endif
#ifdef __NR_pipe2
      SC_ALLOW(pipe2),
#endif
#ifdef __NR_poll
      SC_ALLOW(poll),
#endif
#ifdef __NR_pread
      SC_ALLOW(pread),
#endif
#ifdef __NR_preadv
      SC_ALLOW(preadv),
#endif
#ifdef __NR_pwrite
      SC_ALLOW(pwrite),
#endif
#ifdef __NR_pwritev
      SC_ALLOW(pwritev),
#endif
#ifdef __NR_read
      SC_ALLOW(read),
#endif
#ifdef __NR_readv
      SC_ALLOW(readv),
#endif
#ifdef __NR_recvfrom
      SC_ALLOW(recvfrom),
#endif
#ifdef __NR_recvmsg
      SC_ALLOW(recvmsg),
#endif
#ifdef __NR_select
      SC_ALLOW(select),
#endif
#ifdef __NR_sendmsg
      SC_ALLOW(sendmsg),
#endif
#ifdef __NR_sendto
      SC_ALLOW(sendto),
#endif
#ifdef __NR_setitimer
      SC_ALLOW(setitimer),
#endif
#ifdef __NR_shutdown
      SC_ALLOW(shutdown),
#endif
#ifdef __NR_sigaction
      SC_ALLOW(sigaction),
#endif
#ifdef __NR_sigprocmask
      SC_ALLOW(sigprocmask),
#endif
#ifdef __NR_sigreturn
      SC_ALLOW(sigreturn),
#endif
#ifdef __NR_socketpair
      SC_ALLOW(socketpair),
#endif
#ifdef __NR_umask
      SC_ALLOW(umask),
#endif
#ifdef __NR_wait4
      SC_ALLOW(wait4),
#endif
#ifdef __NR_write
      SC_ALLOW(write),
#endif
#ifdef __NR_writev
      SC_ALLOW(writev),
#endif
#ifdef __NR_restart_syscall
      SC_ALLOW(restart_syscall),
#endif

#ifdef __TERMUX__
#ifdef __NR_prctl
      SC_ALLOW(prctl),
#endif
#endif
#ifdef __NR_newfstatat
      SC_ALLOW(newfstatat),
#endif
#ifdef __NR_getdents64
      SC_ALLOW(getdents64),
#endif
#ifdef __NR_getrandom
      SC_ALLOW(getrandom),
#endif

      /* Default deny */
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_FILTER_FAIL)};

  struct sock_fprog prog = {
      .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
      .filter = filter,
  };

  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog))
    return -1;

  return 0;
}
#endif
