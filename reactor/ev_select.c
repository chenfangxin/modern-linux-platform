/*
 * libev select fd activity backend
 *
 * Copyright (c) 2007,2008,2009,2010,2011 Marc Alexander Lehmann <libev@schmorp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 *
 *   1.  Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MER-
 * CHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPE-
 * CIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTH-
 * ERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License ("GPL") version 2 or any later version,
 * in which case the provisions of the GPL are applicable instead of
 * the above. If you wish to allow the use of your version of this file
 * only under the terms of the GPL and not to allow others to use your
 * version of this file under the BSD license, indicate your decision
 * by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL. If you do not delete the
 * provisions above, a recipient may use your version of this file under
 * either the BSD or the GPL.
 */

/* for unix systems */
# include <inttypes.h>
#  include <sys/select.h>

#ifndef EV_SELECT_USE_FD_SET
# ifdef NFDBITS
#  define EV_SELECT_USE_FD_SET 0
# else
#  define EV_SELECT_USE_FD_SET 1
# endif
#endif

#if !EV_SELECT_USE_FD_SET
# define NFDBYTES (NFDBITS / 8)
#endif

#include <string.h>

static void
select_modify (EV_P_ int fd, int oev, int nev)
{
  if (oev == nev)
    return;

  {
#if EV_SELECT_USE_FD_SET

    int handle = fd;

    assert (("libev: fd >= FD_SETSIZE passed to fd_set-based select backend", fd < FD_SETSIZE));

    /* FD_SET is broken on windows (it adds the fd to a set twice or more,
     * which eventually leads to overflows). Need to call it only on changes.
     */
      if (nev & EV_READ)
        FD_SET (handle, (fd_set *)vec_ri);
      else
        FD_CLR (handle, (fd_set *)vec_ri);

      if (nev & EV_WRITE)
        FD_SET (handle, (fd_set *)vec_wi);
      else
        FD_CLR (handle, (fd_set *)vec_wi);

#else

    int     word = fd / NFDBITS;
    fd_mask mask = 1UL << (fd % NFDBITS);

    if (expect_false (vec_max <= word))
      {
        int new_max = word + 1;

        vec_ri = ev_realloc (vec_ri, new_max * NFDBYTES);
        vec_ro = ev_realloc (vec_ro, new_max * NFDBYTES); /* could free/malloc */
        vec_wi = ev_realloc (vec_wi, new_max * NFDBYTES);
        vec_wo = ev_realloc (vec_wo, new_max * NFDBYTES); /* could free/malloc */

        for (; vec_max < new_max; ++vec_max)
          ((fd_mask *)vec_ri) [vec_max] =
          ((fd_mask *)vec_wi) [vec_max] = 0;
      }

    ((fd_mask *)vec_ri) [word] |= mask;
    if (!(nev & EV_READ))
      ((fd_mask *)vec_ri) [word] &= ~mask;

    ((fd_mask *)vec_wi) [word] |= mask;
    if (!(nev & EV_WRITE))
      ((fd_mask *)vec_wi) [word] &= ~mask;
#endif
  }
}

static void
select_poll (EV_P_ ev_tstamp timeout)
{
  struct timeval tv;
  int res;
  int fd_setsize;

  EV_RELEASE_CB;
  EV_TV_SET (tv, timeout);

#if EV_SELECT_USE_FD_SET
  fd_setsize = sizeof (fd_set);
#else
  fd_setsize = vec_max * NFDBYTES;
#endif

  memcpy (vec_ro, vec_ri, fd_setsize);
  memcpy (vec_wo, vec_wi, fd_setsize);

#if EV_SELECT_USE_FD_SET
  fd_setsize = anfdmax < FD_SETSIZE ? anfdmax : FD_SETSIZE;
  res = select (fd_setsize, (fd_set *)vec_ro, (fd_set *)vec_wo, 0, &tv);
#else
  res = select (vec_max * NFDBITS, (fd_set *)vec_ro, (fd_set *)vec_wo, 0, &tv);
#endif
  EV_ACQUIRE_CB;

  if (expect_false (res < 0))
    {
      #ifdef WSABASEERR
      /* on windows, select returns incompatible error codes, fix this */
      if (errno >= WSABASEERR && errno < WSABASEERR + 1000)
        if (errno == WSAENOTSOCK)
          errno = EBADF;
        else
          errno -= WSABASEERR;
      #endif

      if (errno == EBADF)
        fd_ebadf (EV_A);
      else if (errno == ENOMEM && !syserr_cb)
        fd_enomem (EV_A);
      else if (errno != EINTR)
        ev_syserr ("(libev) select");

      return;
    }

#if EV_SELECT_USE_FD_SET

  {
    int fd;

    for (fd = 0; fd < anfdmax; ++fd)
      if (anfds [fd].events)
        {
          int events = 0;
          int handle = fd;

          if (FD_ISSET (handle, (fd_set *)vec_ro)) events |= EV_READ;
          if (FD_ISSET (handle, (fd_set *)vec_wo)) events |= EV_WRITE;

          if (expect_true (events))
            fd_event (EV_A_ fd, events);
        }
  }

#else

  {
    int word, bit;
    for (word = vec_max; word--; )
      {
        fd_mask word_r = ((fd_mask *)vec_ro) [word];
        fd_mask word_w = ((fd_mask *)vec_wo) [word];

        if (word_r || word_w)
          for (bit = NFDBITS; bit--; )
            {
              fd_mask mask = 1UL << bit;
              int events = 0;

              events |= word_r & mask ? EV_READ  : 0;
              events |= word_w & mask ? EV_WRITE : 0;

              if (expect_true (events))
                fd_event (EV_A_ word * NFDBITS + bit, events);
            }
      }
  }

#endif
}

int inline_size
select_init (EV_P_ int flags)
{
  backend_mintime = 1e-6;
  backend_modify  = select_modify;
  backend_poll    = select_poll;

#if EV_SELECT_USE_FD_SET
  vec_ri  = ev_malloc (sizeof (fd_set)); FD_ZERO ((fd_set *)vec_ri);
  vec_ro  = ev_malloc (sizeof (fd_set));
  vec_wi  = ev_malloc (sizeof (fd_set)); FD_ZERO ((fd_set *)vec_wi);
  vec_wo  = ev_malloc (sizeof (fd_set));
#else
  vec_max = 0;
  vec_ri  = 0;
  vec_ro  = 0;
  vec_wi  = 0;
  vec_wo  = 0;
#endif

  return EVBACKEND_SELECT;
}

void inline_size
select_destroy (EV_P)
{
  ev_free (vec_ri);
  ev_free (vec_ro);
  ev_free (vec_wi);
  ev_free (vec_wo);
}

