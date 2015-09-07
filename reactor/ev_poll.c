#include <poll.h>

static void pollidx_init (int *base, int count)
{
  /* consider using memset (.., -1, ...), which is practically guaranteed
   * to work on all systems implementing poll */
  while (count--)
    *base++ = -1;
}

static void poll_modify (struct ev_loop *loop, int fd, int oev, int nev)
{
  int idx;

  if (oev == nev)
    return;

  array_needsize (int, loop->pollidxs, loop->pollidxmax, fd + 1, pollidx_init);

  idx = loop->pollidxs [fd];

  if (idx < 0) {/* need to allocate a new pollfd */
    
      loop->pollidxs [fd] = idx = loop->pollcnt++;
      array_needsize (struct pollfd, loop->polls, loop->pollmax, loop->pollcnt, EMPTY2);
      loop->polls [idx].fd = fd;
  }

  assert (loop->polls [idx].fd == fd);

  if (nev) {
    loop->polls [idx].events = (nev & EV_READ ? POLLIN : 0) | (nev & EV_WRITE ? POLLOUT : 0);
  } else {/* remove pollfd */
      loop->pollidxs [fd] = -1;

      if (expect_true (idx < --loop->pollcnt)) {
          loop->polls [idx] = loop->polls [loop->pollcnt];
          loop->pollidxs [loop->polls [idx].fd] = idx;
        }
    }
}

static void poll_poll (struct ev_loop *loop, ev_tstamp timeout)
{
  struct pollfd *p;
  int res;
  
  EV_RELEASE_CB;
  res = poll (loop->polls, loop->pollcnt, timeout * 1e3);
  EV_ACQUIRE_CB;

  if (expect_false (res < 0)) {
      if (errno == EBADF)
        fd_ebadf (loop);
      else if (errno == ENOMEM && !syserr_cb)
        fd_enomem (loop);
      else if (errno != EINTR)
        ev_syserr ("(libev) poll");
    } else {
    for (p = loop->polls; res; ++p) {
        assert (("libev: poll() returned illegal result, broken BSD kernel?", p < loop->polls + loop->pollcnt));

        if (expect_false (p->revents)) {/* this expect is debatable */
          
            --res;

            if (expect_false (p->revents & POLLNVAL))
              fd_kill (loop, p->fd);
            else
              fd_event (loop, p->fd,
                (p->revents & (POLLOUT | POLLERR | POLLHUP) ? EV_WRITE : 0)
                | (p->revents & (POLLIN | POLLERR | POLLHUP) ? EV_READ : 0)
              );
          }
      }
	}
}

static int poll_init (struct ev_loop *loop, int flags)
{
  loop->backend_mintime = 1e-3;
  loop->backend_modify  = poll_modify;
  loop->backend_poll    = poll_poll;

  loop->pollidxs = 0; loop->pollidxmax = 0;
  loop->polls = 0; loop->pollmax    = 0; loop->pollcnt = 0;

  return EVBACKEND_POLL;
}

static void poll_destroy (struct ev_loop *loop)
{
  ev_free (loop->pollidxs);
  ev_free (loop->polls);
}

