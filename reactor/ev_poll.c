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

  array_needsize (int, pollidxs, pollidxmax, fd + 1, pollidx_init);

  idx = pollidxs [fd];

  if (idx < 0) /* need to allocate a new pollfd */
    {
      pollidxs [fd] = idx = pollcnt++;
      array_needsize (struct pollfd, polls, pollmax, pollcnt, EMPTY2);
      polls [idx].fd = fd;
    }

  assert (polls [idx].fd == fd);

  if (nev)
    polls [idx].events =
        (nev & EV_READ ? POLLIN : 0)
        | (nev & EV_WRITE ? POLLOUT : 0);
  else /* remove pollfd */
    {
      pollidxs [fd] = -1;

      if (expect_true (idx < --pollcnt))
        {
          polls [idx] = polls [pollcnt];
          pollidxs [polls [idx].fd] = idx;
        }
    }
}

static void
poll_poll (struct ev_loop *loop, ev_tstamp timeout)
{
  struct pollfd *p;
  int res;
  
  EV_RELEASE_CB;
  res = poll (polls, pollcnt, timeout * 1e3);
  EV_ACQUIRE_CB;

  if (expect_false (res < 0))
    {
      if (errno == EBADF)
        fd_ebadf (loop);
      else if (errno == ENOMEM && !syserr_cb)
        fd_enomem (loop);
      else if (errno != EINTR)
        ev_syserr ("(libev) poll");
    }
  else
    for (p = polls; res; ++p)
      {
        assert (("libev: poll() returned illegal result, broken BSD kernel?", p < polls + pollcnt));

        if (expect_false (p->revents)) /* this expect is debatable */
          {
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

static int poll_init (struct ev_loop *loop, int flags)
{
  backend_mintime = 1e-3;
  backend_modify  = poll_modify;
  backend_poll    = poll_poll;

  pollidxs = 0; pollidxmax = 0;
  polls    = 0; pollmax    = 0; pollcnt = 0;

  return EVBACKEND_POLL;
}

static void poll_destroy (struct ev_loop *loop)
{
  ev_free (pollidxs);
  ev_free (polls);
}

