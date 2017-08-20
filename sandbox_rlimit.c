/* Copyright (c) 2017, Michael Santos <michael.santos@gmail.com>
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
#ifdef TREP_SANDBOX_rlimit
#include <sys/time.h>
#include <sys/resource.h>

    int
trep_sandbox_init()
{
    struct rlimit rl_zero = {0};

#ifdef RLIMIT_NPROC
    if (setrlimit(RLIMIT_NPROC, &rl_zero) < 0)
        return -1;
#endif

    return 0;
}

    int
trep_sandbox_stdin()
{
    struct rlimit rl_zero = {0};

#ifdef RLIMIT_NOFILE
    if (setrlimit(RLIMIT_NOFILE, &rl_zero) < 0)
        return -1;
#endif

	return 0;
}
#endif
