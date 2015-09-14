/*
    Copyright (c) 2013 Martin Sustrik  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "../nn.h"

#include "../utils/alloc.h"
#include "../utils/fast.h"
#include "../utils/err.h"

#include <poll.h>
#include <stddef.h>

int nn_poll (struct nn_pollfd *fds, int nfds, int timeout)
{
    int rc;
    int i;
    int pos;
    int fd;
    int res;
    size_t sz;
    struct pollfd *pfd;

    /*  Construct a pollset to be used with OS-level 'poll' function. */
    pfd = nn_alloc (sizeof (struct pollfd) * nfds * 2, "pollset");
    alloc_assert (pfd);
    pos = 0;
    for (i = 0; i != nfds; ++i) {
        if (fds [i].events & NN_POLLIN) {
            sz = sizeof (fd);
            rc = nn_getsockopt (fds [i].fd, NN_SOL_SOCKET, NN_RCVFD, &fd, &sz);
            if (nn_slow (rc < 0)) {
                nn_free (pfd);
                errno = -rc;
                return -1;
            }
            nn_assert (sz == sizeof (fd));
            pfd [pos].fd = fd;
            pfd [pos].events = POLLIN;
            ++pos;
        }
        if (fds [i].events & NN_POLLOUT) {
            sz = sizeof (fd);
            rc = nn_getsockopt (fds [i].fd, NN_SOL_SOCKET, NN_SNDFD, &fd, &sz);
            if (nn_slow (rc < 0)) {
                nn_free (pfd);
                errno = -rc;
                return -1;
            }
            nn_assert (sz == sizeof (fd));
            pfd [pos].fd = fd;
            pfd [pos].events = POLLIN;
            ++pos;
        }
    }    

    /*  Do the polling itself. */
    rc = poll (pfd, pos, timeout);
    if (nn_slow (rc <= 0)) {
        res = errno;
        nn_free (pfd);
        errno = res;
        return rc;
    }

    /*  Move the results from OS-level poll to nn_poll's pollset. */
    res = 0;
    pos = 0;
    for (i = 0; i != nfds; ++i) {
        fds [i].revents = 0;
        if (fds [i].events & NN_POLLIN) {
            if (pfd [pos].revents & POLLIN)
                fds [i].revents |= NN_POLLIN;
            ++pos;
        }
        if (fds [i].events & NN_POLLOUT) {
            if (pfd [pos].revents & POLLIN)
                fds [i].revents |= NN_POLLOUT;
            ++pos;
        }
        if (fds [i].revents)
            ++res;
    }

    nn_free (pfd);
    return res;
}
