/* this big block deduces configuration from config.h */
#include "config.h"

#ifndef EV_USE_NANOSLEEP
#define EV_USE_NANOSLEEP EV_FEATURE_OS
#endif

#ifndef EV_USE_SELECT
#define EV_USE_SELECT EV_FEATURE_BACKENDS
#endif

#ifndef EV_USE_POLL
#define EV_USE_POLL EV_FEATURE_BACKENDS
#endif
   
#ifndef EV_USE_EPOLL
#define EV_USE_EPOLL EV_FEATURE_BACKENDS
#endif
   
#ifndef EV_USE_INOTIFY
#define EV_USE_INOTIFY EV_FEATURE_OS
#endif

#ifndef EV_USE_SIGNALFD
#define EV_USE_SIGNALFD EV_FEATURE_OS
#endif

#ifndef EV_USE_EVENTFD
#define EV_USE_EVENTFD EV_FEATURE_OS
#endif
 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stddef.h>

#include <stdio.h>

#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>

#include <signal.h>

# include "ev.h"

#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

/* try to deduce the maximum number of signals on this platform */
#if defined EV_NSIG
/* use what's provided */
#elif defined NSIG
# define EV_NSIG (NSIG)
#elif defined _NSIG
# define EV_NSIG (_NSIG)
#elif defined SIGMAX
# define EV_NSIG (SIGMAX+1)
#elif defined SIG_MAX
# define EV_NSIG (SIG_MAX+1)
#elif defined _SIG_MAX
# define EV_NSIG (_SIG_MAX+1)
#elif defined MAXSIG
# define EV_NSIG (MAXSIG+1)
#elif defined MAX_SIG
# define EV_NSIG (MAX_SIG+1)
#elif defined SIGARRAYSIZE
# define EV_NSIG (SIGARRAYSIZE) /* Assume ary[SIGARRAYSIZE] */
#elif defined _sys_nsig
# define EV_NSIG (_sys_nsig) /* Solaris 2.5 */
#else
# define EV_NSIG (8 * sizeof (sigset_t) + 1)
#endif

#ifndef EV_USE_NANOSLEEP
# if _POSIX_C_SOURCE >= 199309L
#  define EV_USE_NANOSLEEP EV_FEATURE_OS
# else
#  define EV_USE_NANOSLEEP 0
# endif
#endif

#ifndef EV_USE_SELECT
# define EV_USE_SELECT EV_FEATURE_BACKENDS
#endif

#ifndef EV_USE_POLL
#  define EV_USE_POLL EV_FEATURE_BACKENDS
#endif

#ifndef EV_USE_EPOLL
# if __linux && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 4))
#  define EV_USE_EPOLL EV_FEATURE_BACKENDS
# else
#  define EV_USE_EPOLL 0
# endif
#endif

#ifndef EV_USE_INOTIFY
# if __linux && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 4))
#  define EV_USE_INOTIFY EV_FEATURE_OS
# else
#  define EV_USE_INOTIFY 0
# endif
#endif

#ifndef EV_PID_HASHSIZE
# define EV_PID_HASHSIZE EV_FEATURE_DATA ? 16 : 1
#endif

#ifndef EV_INOTIFY_HASHSIZE
# define EV_INOTIFY_HASHSIZE EV_FEATURE_DATA ? 16 : 1
#endif

#if 0 /* debugging */
# define EV_VERIFY 3
# define EV_HEAP_CACHE_AT 1
#endif

#ifndef EV_VERIFY
# define EV_VERIFY (EV_FEATURE_API ? 1 : 0)
#endif

#ifndef EV_HEAP_CACHE_AT
# define EV_HEAP_CACHE_AT EV_FEATURE_DATA
#endif

/* on linux, we can use a (slow) syscall to avoid a dependency on pthread, */
/* which makes programs even slower. might work on other unices, too. */

/* this block fixes any misconfiguration where we know we run into trouble otherwise */

#if !EV_STAT_ENABLE
# undef EV_USE_INOTIFY
# define EV_USE_INOTIFY 0
#endif

#if !EV_USE_NANOSLEEP
/* hp-ux has it in sys/time.h, which we unconditionally include above */
#  include <sys/select.h>
#endif

#if EV_USE_INOTIFY
# include <sys/statfs.h>
# include <sys/inotify.h>
/* some very old inotify.h headers don't have IN_DONT_FOLLOW */
# ifndef IN_DONT_FOLLOW
#  undef EV_USE_INOTIFY
#  define EV_USE_INOTIFY 0
# endif
#endif

#if EV_USE_EVENTFD
/* our minimum requirement is glibc 2.7 which has the stub, but not the header */
# include <stdint.h>
# ifndef EFD_NONBLOCK
#  define EFD_NONBLOCK O_NONBLOCK
# endif
# ifndef EFD_CLOEXEC
#  ifdef O_CLOEXEC
#   define EFD_CLOEXEC O_CLOEXEC
#  else
#   define EFD_CLOEXEC 02000000
#  endif
# endif
int (eventfd) (unsigned int initval, int flags);
#endif

#if EV_USE_SIGNALFD
/* our minimum requirement is glibc 2.7 which has the stub, but not the header */
# include <stdint.h>
# ifndef SFD_NONBLOCK
#  define SFD_NONBLOCK O_NONBLOCK
# endif
# ifndef SFD_CLOEXEC
#  ifdef O_CLOEXEC
#   define SFD_CLOEXEC O_CLOEXEC
#  else
#   define SFD_CLOEXEC 02000000
#  endif
# endif
int signalfd (int fd, const sigset_t *mask, int flags);

struct signalfd_siginfo
{
  uint32_t ssi_signo;
  char pad[128 - sizeof (uint32_t)];
};
#endif

#if EV_VERIFY >= 3
# define EV_FREQUENT_CHECK ev_verify (loop)
#else
# define EV_FREQUENT_CHECK do { } while (0)
#endif

/*
 * This is used to work around floating point rounding problems.
 * This value is good at least till the year 4000.
 */
#define MIN_INTERVAL  0.0001220703125 /* 1/2**13, good till 4000 */

#define MIN_TIMEJUMP  1. /* minimum timejump that gets detected (if monotonic clock available) */
#define MAX_BLOCKTIME 59.743 /* never wait longer than this time (to detect time jumps) */

#define EV_TV_SET(tv,t) do { tv.tv_sec = (long)t; tv.tv_usec = (long)((t - tv.tv_sec) * 1e6); } while (0)
#define EV_TS_SET(ts,t) do { ts.tv_sec = (long)t; ts.tv_nsec = (long)((t - ts.tv_sec) * 1e9); } while (0)

#include "libecb.h"

#if ECB_MEMORY_FENCE_NEEDS_PTHREADS
/* if your architecture doesn't need memory fences, e.g. because it is
 * single-cpu/core, or if you use libev in a project that doesn't use libev
 * from multiple threads, then you can define ECB_AVOID_PTHREADS when compiling
 * libev, in which cases the memory fences become nops.
 * alternatively, you can remove this #error and link against libpthread,
 * which will then provide the memory fences.
 */
# error "memory fences not defined for your architecture, please report"
#endif

#ifndef ECB_MEMORY_FENCE
# define ECB_MEMORY_FENCE do { } while (0)
# define ECB_MEMORY_FENCE_ACQUIRE ECB_MEMORY_FENCE
# define ECB_MEMORY_FENCE_RELEASE ECB_MEMORY_FENCE
#endif

#define expect_false(cond) ecb_expect_false (cond)
#define expect_true(cond)  ecb_expect_true  (cond)

#define NUMPRI (EV_MAXPRI - EV_MINPRI + 1)

#if EV_MINPRI == EV_MAXPRI
# define ABSPRI(w) (((W)w), 0)
#else
# define ABSPRI(w) (((W)w)->priority - EV_MINPRI)
#endif

#define EMPTY       /* required for microsofts broken pseudo-c compiler */
#define EMPTY2(a,b) /* used to suppress some warnings */

typedef ev_watcher *W;
typedef ev_watcher_list *WL;
typedef ev_watcher_time *WT;

#define ev_active(w) ((W)(w))->active
#define ev_at(w) ((WT)(w))->at

/*****************************************************************************/

/* define a suitable floor function (only used by periodics atm) */
# include <math.h>
# define ev_floor(v) floor (v)

/*****************************************************************************/

#ifdef __linux
# include <sys/utsname.h>
#endif

static unsigned int ev_linux_version (void)
{
#ifdef __linux
  unsigned int v = 0;
  struct utsname buf;
  int i;
  char *p = buf.release;

  if (uname (&buf))
    return 0;

  for (i = 3+1; --i; ){
      unsigned int c = 0;

      for (;;) {
          if (*p >= '0' && *p <= '9') {
            c = c * 10 + *p++ - '0';
		  } else {
              p += *p == '.';
              break;
            }
        }

      v = (v << 8) | c;
    }

  return v;
#else
  return 0;
#endif
}

/*****************************************************************************/

#if EV_AVOID_STDIO
static void ev_printerr (const char *msg)
{
  write (STDERR_FILENO, msg, strlen (msg));
}
#endif

static void (*syserr_cb)(const char *msg)  ;

void ev_set_syserr_cb (void (*cb)(const char *msg)  )  
{
  syserr_cb = cb;
}

static void ev_syserr (const char *msg)
{
  if (!msg)
    msg = "(libev) system error";

  if (syserr_cb)
    syserr_cb (msg);
  else
    {
#if EV_AVOID_STDIO
      ev_printerr (msg);
      ev_printerr (": ");
      ev_printerr (strerror (errno));
      ev_printerr ("\n");
#else
      perror (msg);
#endif
      abort ();
    }
}

static void * ev_realloc_emul (void *ptr, long size)  
{
  /* some systems, notably openbsd and darwin, fail to properly
   * implement realloc (x, 0) (as required by both ansi c-89 and
   * the single unix specification, so work around them here.
   * recently, also (at least) fedora and debian started breaking it,
   * despite documenting it otherwise.
   */

  if (size)
    return realloc (ptr, size);

  free (ptr);
  return 0;
}

static void *(*alloc)(void *ptr, long size)   = ev_realloc_emul;

void ev_set_allocator (void *(*cb)(void *ptr, long size)  )  
{
  alloc = cb;
}

static void * ev_realloc (void *ptr, long size)
{
  ptr = alloc (ptr, size);

  if (!ptr && size)
    {
#if EV_AVOID_STDIO
      ev_printerr ("(libev) memory allocation failed, aborting.\n");
#else
      fprintf (stderr, "(libev) cannot allocate %ld bytes, aborting.", size);
#endif
      abort ();
    }

  return ptr;
}

#define ev_malloc(size) ev_realloc (0, (size))
#define ev_free(ptr)    ev_realloc ((ptr), 0)

/*****************************************************************************/

/* set in reify when reification needed */
#define EV_ANFD_REIFY 1

/* file descriptor info structure */
typedef struct {
  WL head;
  unsigned char events; /* the events watched for */
  unsigned char reify;  /* flag set when this ANFD needs reification (EV_ANFD_REIFY, EV__IOFDSET) */
  unsigned char emask;  /* the epoll backend stores the actual kernel mask in here */
  unsigned char unused;
#if EV_USE_EPOLL
  unsigned int egen;    /* generation counter to counter epoll bugs */
#endif
} ANFD;

/* stores the pending event set for a given watcher */
typedef struct {
  W w;
  int events; /* the pending event set for the given watcher */
} ANPENDING;

#if EV_USE_INOTIFY
/* hash table entry per inotify-id */
typedef struct {
  WL head;
} ANFS;
#endif

/* Heap Entry */
#if EV_HEAP_CACHE_AT
/* a heap element */
typedef struct {
  ev_tstamp at;
  WT w;
} ANHE;

  #define ANHE_w(he)        (he).w     /* access watcher, read-write */
  #define ANHE_at(he)       (he).at    /* access cached at, read-only */
  #define ANHE_at_cache(he) (he).at = (he).w->at /* update at from watcher */
#else
  /* a heap element */
  typedef WT ANHE;

  #define ANHE_w(he)        (he)
  #define ANHE_at(he)       (he)->at
  #define ANHE_at_cache(he)
#endif

struct ev_loop{
	ev_tstamp ev_rt_now;
	ev_tstamp now_floor;
	ev_tstamp mn_now;
	ev_tstamp rtmn_diff;

	W * rfeeds;
	int rfeedmax;
	int rfeedcnt;
	
	ANPENDING *pendings[NUMPRI];
	int pendingmax[NUMPRI];
	int pendingcnt[NUMPRI];
	int pendingpri;
	ev_prepare pending_w;
	
	ev_tstamp io_blocktime;
	ev_tstamp timeout_blocktime;
	
	int backend;
	int activecnt;
	EV_ATOMIC_T loop_done;

	int backend_fd;
	ev_tstamp backend_mintime;
	void (*backend_modify)(struct ev_loop *loop, int fd, int oev, int nev);
	void (*backend_poll)(struct ev_loop *loop, ev_tstamp timeout);

	ANFD *anfds;
	int anfdmax;

	int evpipe[2];
	ev_io pipe_w;
	EV_ATOMIC_T pipe_write_wanted;
	EV_ATOMIC_T pipe_write_skipped;

	pid_t curpid;

	char postfork;

#if EV_USE_SELECT || EV_GENWRAP
	void * vec_ri;
	void * vec_ro;
	void * vec_wi;
	void * vec_wo;
	void * vec_eo;
	int vec_max;
#endif

#if EV_USE_POLL || EV_GENWRAP
	struct pollfd * polls;
	int pollmax;
	int pollcnt;
	int *pollidxs;
	int pollidxmax;
#endif

#if EV_USE_EPOLL || EV_GENWRAP
	struct epoll_event * epoll_events;
	int epoll_eventmax;
	int * epoll_eperms;
	int epoll_epermcnt;
	int epoll_epermmax;
#endif

	int *fdchanges;
	int fdchangemax;
	int fdchangecnt;

	ANHE * timers;
	int timermax;
	int timercnt;

#if EV_PERIODIC_ENABLE || EV_GENWRAP
	ANHE * periodics;
	int periodicmax;
	int periodiccnt;
#endif

#if EV_IDLE_ENABLE || EV_GENWRAP
	ev_idle **idles[NUMPRI];
	int idlemax[NUMPRI];
	int idlecnt [NUMPRI];
#endif

	int idleall; 

	struct ev_prepare **prepares;
	int preparemax;
	int preparecnt;

	struct ev_check **checks;
	int checkmax;
	int checkcnt;

#if EV_FORK_ENABLE || EV_GENWRAP
	struct ev_fork ** forks;
	int forkmax;
	int forkcnt;
#endif

#if EV_CLEANUP_ENABLE || EV_GENWRAP
	struct ev_cleanup ** cleanups;
	int cleanupmax;
	int cleanupcnt;
#endif

#if EV_ASYNC_ENABLE || EV_GENWRAP
	EV_ATOMIC_T async_pending;
	struct ev_async ** asyncs;
	int asyncmax;
	int asynccnt;
#endif

#if EV_USE_INOTIFY || EV_GENWRAP
	int fs_fd;
	ev_io fs_w;
	char fs_2625;
	ANFS fs_hash [EV_INOTIFY_HASHSIZE];
#endif

	EV_ATOMIC_T sig_pending;
#if EV_USE_SIGNALFD || EV_GENWRAP
	int sigfd;
	ev_io sigfd_w;
	sigset_t sigfd_set;
#endif

	unsigned int origflags;

#if EV_FEATURE_API || EV_GENWRAP
	unsigned int loop_count;
	unsigned int loop_depth;

	void * userdata;

	void (*release_cb)(struct ev_loop *loop);
	void (*acquire_cb)(struct ev_loop *loop);
	ev_loop_callback invoke_cb;
#endif
};

static struct ev_loop default_loop_struct;
struct ev_loop *ev_default_loop_ptr = 0; /* needs to be initialised to make it a definition despite extern */

#if EV_FEATURE_API
# define EV_RELEASE_CB if (expect_false (loop->release_cb)) loop->release_cb (loop)
# define EV_ACQUIRE_CB if (expect_false (loop->acquire_cb)) loop->acquire_cb (loop)
# define EV_INVOKE_PENDING 	loop->invoke_cb(loop)
#else
# define EV_RELEASE_CB (void)0
# define EV_ACQUIRE_CB (void)0
# define EV_INVOKE_PENDING ev_invoke_pending (loop)
#endif

#define EVBREAK_RECURSE 0x80

/*****************************************************************************/
#ifndef EV_HAVE_EV_TIME
ev_tstamp ev_time (void)  
{
  struct timeval tv;
  gettimeofday (&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}
#endif

static ev_tstamp get_clock (void)
{
  return ev_time ();
}

ev_tstamp ev_now (struct ev_loop *loop)  
{
  return loop->ev_rt_now;
}

void ev_sleep (ev_tstamp delay)  
{
  if (delay > 0.)
    {
#if EV_USE_NANOSLEEP
      struct timespec ts;

      EV_TS_SET (ts, delay);
      nanosleep (&ts, 0);
#else
      struct timeval tv;

      /* here we rely on sys/time.h + sys/types.h + unistd.h providing select */
      /* something not guaranteed by newer posix versions, but guaranteed */
      /* by older ones */
      EV_TV_SET (tv, delay);
      select (0, 0, 0, 0, &tv);
#endif
    }
}

/*****************************************************************************/

#define MALLOC_ROUND 4096L /* prefer to allocate in chunks of this size, must be 2**n and >> 4 longs */

/* find a suitable new size for the given array, */
/* hopefully by rounding to a nice-to-malloc size */
static int array_nextsize (int elem, int cur, int cnt)
{
  int ncur = cur + 1;

  do
    ncur <<= 1;
  while (cnt > ncur);

  /* if size is large, round to MALLOC_ROUND - 4 * longs to accommodate malloc overhead */
  if ((elem * ncur) > (int)(MALLOC_ROUND - 4*sizeof(void *)))
    {
      ncur *= elem;
      ncur = (ncur + elem + (MALLOC_ROUND - 1) + sizeof (void *) * 4) & ~(MALLOC_ROUND - 1);
      ncur = ncur - sizeof (void *) * 4;
      ncur /= elem;
    }

  return ncur;
}

static void * array_realloc (int elem, void *base, int *cur, int cnt)
{
  *cur = array_nextsize (elem, *cur, cnt);
  return ev_realloc (base, elem * *cur);
}

#define array_init_zero(base,count)	\
  memset ((void *)(base), 0, sizeof (*(base)) * (count))

#define array_needsize(type,base,cur,cnt,init)			\
  if (expect_false ((cnt) > (cur)))	{			\
      int ocur_ = (cur);					\
      (base) = (type *)array_realloc				\
         (sizeof (type), (base), &(cur), (cnt));		\
      init ((base) + (ocur_), (cur) - ocur_);			\
   }

/*****************************************************************************/
/* dummy callback for pending events */
static void pendingcb (struct ev_loop *loop, ev_prepare *w, int revents)
{

}

void ev_feed_event (struct ev_loop *loop, void *w, int revents)  
{
  W w_ = (W)w;
  int pri = ABSPRI (w_);

  if (expect_false (w_->pending)) {
    loop->pendings [pri][w_->pending - 1].events |= revents;
  } else {
      w_->pending = ++loop->pendingcnt[pri];
      array_needsize (ANPENDING, loop->pendings[pri], loop->pendingmax [pri], w_->pending, EMPTY2);
      loop->pendings [pri][w_->pending - 1].w      = w_;
      loop->pendings [pri][w_->pending - 1].events = revents;
  }

  loop->pendingpri = NUMPRI - 1;
}

static void feed_reverse (struct ev_loop *loop, W w)
{
  array_needsize (W, loop->rfeeds, loop->rfeedmax, loop->rfeedcnt + 1, EMPTY2);
  loop->rfeeds [loop->rfeedcnt++] = w;
}

static void feed_reverse_done (struct ev_loop *loop, int revents)
{
  do{
    ev_feed_event (loop, loop->rfeeds[--loop->rfeedcnt], revents);
  }while (loop->rfeedcnt);
}

static void queue_events (struct ev_loop *loop, W *events, int eventcnt, int type)
{
  int i;

  for (i = 0; i < eventcnt; ++i)
    ev_feed_event (loop, events [i], type);
}

/*****************************************************************************/
static void fd_event_nocheck (struct ev_loop *loop, int fd, int revents)
{
  ANFD *anfd = loop->anfds + fd;
  ev_io *w;

  for (w = (ev_io *)anfd->head; w; w = (ev_io *)((WL)w)->next)
    {
      int ev = w->events & revents;

      if (ev)
        ev_feed_event (loop, (W)w, ev);
    }
}

/* do not submit kernel events for fds that have reify set */
/* because that means they changed while we were polling for new events */
static void fd_event (struct ev_loop *loop, int fd, int revents)
{
  ANFD *anfd = loop->anfds + fd;

  if (expect_true (!anfd->reify))
    fd_event_nocheck (loop, fd, revents);
}

void ev_feed_fd_event (struct ev_loop *loop, int fd, int revents)  
{
  if (fd >= 0 && fd < loop->anfdmax)
    fd_event_nocheck (loop, fd, revents);
}

/* make sure the external fd watch events are in-sync */
/* with the kernel/libev internal state */
static void fd_reify (struct ev_loop *loop)
{
  int i;

  for (i = 0; i < loop->fdchangecnt; ++i)
    {
      int fd = loop->fdchanges [i];
      ANFD *anfd = loop->anfds + fd;
      ev_io *w;

      unsigned char o_events = anfd->events;
      unsigned char o_reify  = anfd->reify;

      anfd->reify  = 0;

      /*if (expect_true (o_reify & EV_ANFD_REIFY)) probably a deoptimisation */
        {
          anfd->events = 0;

          for (w = (ev_io *)anfd->head; w; w = (ev_io *)((WL)w)->next)
            anfd->events |= (unsigned char)w->events;

          if (o_events != anfd->events)
            o_reify = EV__IOFDSET; /* actually |= */
        }

      if (o_reify & EV__IOFDSET)
        loop->backend_modify (loop, fd, o_events, anfd->events);
    }

  loop->fdchangecnt = 0;
}

/* something about the given fd changed */
static void fd_change (struct ev_loop *loop, int fd, int flags)
{
  unsigned char reify = loop->anfds [fd].reify;
  loop->anfds [fd].reify |= flags;

  if (expect_true (!reify)) {
      ++loop->fdchangecnt;
      array_needsize (int, loop->fdchanges, loop->fdchangemax, loop->fdchangecnt, EMPTY2);
      loop->fdchanges [loop->fdchangecnt - 1] = fd;
    }
}

/* the given fd is invalid/unusable, so make sure it doesn't hurt us anymore */
static void fd_kill (struct ev_loop *loop, int fd)
{
  ev_io *w;

  while ((w = (ev_io *)loop->anfds [fd].head))
    {
      ev_io_stop (loop, w);
      ev_feed_event (loop, (W)w, EV_ERROR | EV_READ | EV_WRITE);
    }
}

/* check whether the given fd is actually valid, for error recovery */
static int fd_valid (int fd)
{
  return fcntl (fd, F_GETFD) != -1;
}

/* called on EBADF to verify fds */
static void fd_ebadf (struct ev_loop *loop)
{
  int fd;

  for (fd = 0; fd < loop->anfdmax; ++fd)
    if (loop->anfds [fd].events)
      if (!fd_valid (fd) && errno == EBADF)
        fd_kill (loop, fd);
}

/* called on ENOMEM in select/poll to kill some fds and retry */
static void fd_enomem (struct ev_loop *loop)
{
  int fd;

  for (fd = loop->anfdmax; fd--; )
    if (loop->anfds [fd].events)
      {
        fd_kill (loop, fd);
        break;
      }
}

/* usually called after fork if backend needs to re-arm all fds from scratch */
static void fd_rearm_all (struct ev_loop *loop)
{
  int fd;

  for (fd = 0; fd < loop->anfdmax; ++fd)
    if (loop->anfds [fd].events) {
        loop->anfds [fd].events = 0;
        loop->anfds [fd].emask  = 0;
        fd_change (loop, fd, EV__IOFDSET | EV_ANFD_REIFY);
      }
}

/* used to prepare libev internal fd's */
/* this is not fork-safe */
static void fd_intern (int fd)
{
  fcntl (fd, F_SETFD, FD_CLOEXEC);
  fcntl (fd, F_SETFL, O_NONBLOCK);
}

/*****************************************************************************/

/*
 * the heap functions want a real array index. array index 0 is guaranteed to not
 * be in-use at any time. the first heap entry is at array [HEAP0]. DHEAP gives
 * the branching factor of the d-tree.
 */
#define DHEAP 4
#define HEAP0 (DHEAP - 1) /* index of first element in heap */
#define HPARENT(k) ((((k) - HEAP0 - 1) / DHEAP) + HEAP0)
#define UPHEAP_DONE(p,k) ((p) == (k))

/* away from the root */
static void downheap (ANHE *heap, int N, int k)
{
  ANHE he = heap [k];
  ANHE *E = heap + N + HEAP0;

  for (;;)
    {
      ev_tstamp minat;
      ANHE *minpos;
      ANHE *pos = heap + DHEAP * (k - HEAP0) + HEAP0 + 1;

      /* find minimum child */
      if (expect_true (pos + DHEAP - 1 < E))
        {
          /* fast path */                               (minpos = pos + 0), (minat = ANHE_at (*minpos));
          if (               ANHE_at (pos [1]) < minat) (minpos = pos + 1), (minat = ANHE_at (*minpos));
          if (               ANHE_at (pos [2]) < minat) (minpos = pos + 2), (minat = ANHE_at (*minpos));
          if (               ANHE_at (pos [3]) < minat) (minpos = pos + 3), (minat = ANHE_at (*minpos));
        }
      else if (pos < E)
        {
          /* slow path */                               (minpos = pos + 0), (minat = ANHE_at (*minpos));
          if (pos + 1 < E && ANHE_at (pos [1]) < minat) (minpos = pos + 1), (minat = ANHE_at (*minpos));
          if (pos + 2 < E && ANHE_at (pos [2]) < minat) (minpos = pos + 2), (minat = ANHE_at (*minpos));
          if (pos + 3 < E && ANHE_at (pos [3]) < minat) (minpos = pos + 3), (minat = ANHE_at (*minpos));
        }
      else
        break;

      if (ANHE_at (he) <= minat)
        break;

      heap [k] = *minpos;
      ev_active (ANHE_w (*minpos)) = k;

      k = minpos - heap;
    }

  heap [k] = he;
  ev_active (ANHE_w (he)) = k;
}

/* towards the root */
static void upheap (ANHE *heap, int k)
{
  ANHE he = heap [k];

  for (;;)
    {
      int p = HPARENT (k);

      if (UPHEAP_DONE (p, k) || ANHE_at (heap [p]) <= ANHE_at (he))
        break;

      heap [k] = heap [p];
      ev_active (ANHE_w (heap [k])) = k;
      k = p;
    }

  heap [k] = he;
  ev_active (ANHE_w (he)) = k;
}

/* move an element suitably so it is in a correct place */
static void adjustheap (ANHE *heap, int N, int k)
{
  if (k > HEAP0 && ANHE_at (heap [k]) <= ANHE_at (heap [HPARENT (k)]))
    upheap (heap, k);
  else
    downheap (heap, N, k);
}

/* rebuild the heap: this function is used only once and executed rarely */
static void reheap (ANHE *heap, int N)
{
  int i;

  /* we don't use floyds algorithm, upheap is simpler and is more cache-efficient */
  /* also, this is easy to implement and correct for both 2-heaps and 4-heaps */
  for (i = 0; i < N; ++i)
    upheap (heap, i + HEAP0);
}

/*****************************************************************************/

/* associate signal watchers to a signal signal */
typedef struct
{
  EV_ATOMIC_T pending;
  struct ev_loop *loop;
  WL head;
} ANSIG;

static ANSIG signals [EV_NSIG - 1];

/*****************************************************************************/
#if EV_SIGNAL_ENABLE || EV_ASYNC_ENABLE
static void evpipe_init (struct ev_loop *loop)
{
  if (!ev_is_active (&loop->pipe_w))
    {
      int fds [2];

# if EV_USE_EVENTFD
      fds [0] = -1;
      fds [1] = eventfd (0, EFD_NONBLOCK | EFD_CLOEXEC);
      if (fds [1] < 0 && errno == EINVAL)
        fds [1] = eventfd (0, 0);

      if (fds [1] < 0)
# endif
        {
          while (pipe (fds))
            ev_syserr ("(libev) error creating signal/async pipe");

          fd_intern (fds [0]);
        }

      loop->evpipe [0] = fds [0];

      if (loop->evpipe [1] < 0){
        loop->evpipe [1] = fds [1]; /* first call, set write fd */
	  }else {
          /* on subsequent calls, do not change evpipe [1] */
          /* so that evpipe_write can always rely on its value. */
          /* this branch does not do anything sensible on windows, */
          /* so must not be executed on windows */

          dup2 (fds [1], loop->evpipe [1]);
          close (fds [1]);
      }

      fd_intern (loop->evpipe [1]);

      ev_io_set (&loop->pipe_w, loop->evpipe [0] < 0 ? loop->evpipe [1] : loop->evpipe [0], EV_READ);
      ev_io_start (loop, &loop->pipe_w);
      ev_unref (loop); /* watcher should not keep loop alive */
    }
}

static void evpipe_write (struct ev_loop *loop, EV_ATOMIC_T *flag)
{
  ECB_MEMORY_FENCE; /* push out the write before this function was called, acquire flag */

  if (expect_true (*flag))
    return;

  *flag = 1;
  ECB_MEMORY_FENCE_RELEASE; /* make sure flag is visible before the wakeup */

  loop->pipe_write_skipped = 1;

  ECB_MEMORY_FENCE; /* make sure pipe_write_skipped is visible before we check pipe_write_wanted */

  if (loop->pipe_write_wanted) {
      int old_errno;

      loop->pipe_write_skipped = 0;
      ECB_MEMORY_FENCE_RELEASE;

      old_errno = errno; /* save errno because write will clobber it */

#if EV_USE_EVENTFD
      if (loop->evpipe [0] < 0)
        {
          uint64_t counter = 1;
          write (loop->evpipe [1], &counter, sizeof (uint64_t));
        }
      else
#endif
        {
          write (loop->evpipe [1], &(loop->evpipe [1]), 1);
        }

      errno = old_errno;
    }
}

/* called whenever the libev signal pipe */
/* got some events (signal, async) */
static void
pipecb (struct ev_loop *loop, ev_io *iow, int revents)
{
  int i;

  if (revents & EV_READ)
    {
#if EV_USE_EVENTFD
      if (loop->evpipe [0] < 0)
        {
          uint64_t counter;
          read (loop->evpipe [1], &counter, sizeof (uint64_t));
        }
      else
#endif
        {
          char dummy[4];
          read (loop->evpipe [0], &dummy, sizeof (dummy));
        }
    }

  loop->pipe_write_skipped = 0;

  ECB_MEMORY_FENCE; /* push out skipped, acquire flags */

#if EV_SIGNAL_ENABLE
  if (loop->sig_pending) {
      loop->sig_pending = 0;

      ECB_MEMORY_FENCE;

      for (i = EV_NSIG - 1; i--; )
        if (expect_false (signals [i].pending))
          ev_feed_signal_event (loop, i + 1);
    }
#endif

#if EV_ASYNC_ENABLE
  if (loop->async_pending)
    {
      loop->async_pending = 0;

      ECB_MEMORY_FENCE;

      for (i = loop->asynccnt; i--; )
        if (loop->asyncs [i]->sent) {
            loop->asyncs [i]->sent = 0;
            ECB_MEMORY_FENCE_RELEASE;
            ev_feed_event (loop, loop->asyncs [i], EV_ASYNC);
          }
    }
#endif
}

/*****************************************************************************/

void
ev_feed_signal (int signum)  
{
  struct ev_loop *loop;
  ECB_MEMORY_FENCE_ACQUIRE;
  loop = signals [signum - 1].loop;

  if (!loop)
    return;

  signals [signum - 1].pending = 1;
  evpipe_write (loop, &loop->sig_pending);
}

static void
ev_sighandler (int signum)
{

  ev_feed_signal (signum);
}

void 
ev_feed_signal_event (struct ev_loop *loop, int signum)  
{
  WL w;

  if (expect_false (signum <= 0 || signum >= EV_NSIG))
    return;

  --signum;

  /* it is permissible to try to feed a signal to the wrong loop */
  /* or, likely more useful, feeding a signal nobody is waiting for */

  if (expect_false (signals [signum].loop != loop))
    return;

  signals [signum].pending = 0;
  ECB_MEMORY_FENCE_RELEASE;

  for (w = signals [signum].head; w; w = w->next)
    ev_feed_event (loop, (W)w, EV_SIGNAL);
}

#if EV_USE_SIGNALFD
static void
sigfdcb (struct ev_loop *loop, ev_io *iow, int revents)
{
  struct signalfd_siginfo si[2], *sip; /* these structs are big */

  for (;;)
    {
      ssize_t res = read (loop->sigfd, si, sizeof (si));

      /* not ISO-C, as res might be -1, but works with SuS */
      for (sip = si; (char *)sip < (char *)si + res; ++sip)
        ev_feed_signal_event (loop, sip->ssi_signo);

      if (res < (ssize_t)sizeof (si))
        break;
    }
}
#endif

#endif

/*****************************************************************************/

#if EV_CHILD_ENABLE
static WL childs [EV_PID_HASHSIZE];

static ev_signal childev;

#ifndef WIFCONTINUED
# define WIFCONTINUED(status) 0
#endif

/* handle a single child status event */
static void
child_reap (struct ev_loop *loop, int chain, int pid, int status)
{
  ev_child *w;
  int traced = WIFSTOPPED (status) || WIFCONTINUED (status);

  for (w = (ev_child *)childs [chain & ((EV_PID_HASHSIZE) - 1)]; w; w = (ev_child *)((WL)w)->next)
    {
      if ((w->pid == pid || !w->pid)
          && (!traced || (w->flags & 1)))
        {
          ev_set_priority (w, EV_MAXPRI); /* need to do it *now*, this *must* be the same prio as the signal watcher itself */
          w->rpid    = pid;
          w->rstatus = status;
          ev_feed_event (loop, (W)w, EV_CHILD);
        }
    }
}

#ifndef WCONTINUED
# define WCONTINUED 0
#endif

/* called on sigchld etc., calls waitpid */
static void childcb (struct ev_loop *loop, ev_signal *sw, int revents)
{
  int pid, status;

  /* some systems define WCONTINUED but then fail to support it (linux 2.4) */
  if (0 >= (pid = waitpid (-1, &status, WNOHANG | WUNTRACED | WCONTINUED)))
    if (!WCONTINUED
        || errno != EINVAL
        || 0 >= (pid = waitpid (-1, &status, WNOHANG | WUNTRACED)))
      return;

  /* make sure we are called again until all children have been reaped */
  /* we need to do it this way so that the callback gets called before we continue */
  ev_feed_event (loop, (W)sw, EV_SIGNAL);

  child_reap (loop, pid, pid, status);
  if ((EV_PID_HASHSIZE) > 1)
    child_reap (loop, 0, pid, status); /* this might trigger a watcher twice, but feed_event catches that */
}
#endif

/*****************************************************************************/

#if EV_USE_EPOLL
# include "ev_epoll.c"
#endif
#if EV_USE_POLL
# include "ev_poll.c"
#endif
#if EV_USE_SELECT
# include "ev_select.c"
#endif

int ev_version_major (void)  
{
  return EV_VERSION_MAJOR;
}

int ev_version_minor (void)  
{
  return EV_VERSION_MINOR;
}

/* return true if we are running with elevated privileges and should ignore env variables */
static int enable_secure (void)
{
  return getuid () != geteuid ()
      || getgid () != getegid ();
}

unsigned int ev_supported_backends (void)  
{
  unsigned int flags = 0;

  if (EV_USE_EPOLL ) flags |= EVBACKEND_EPOLL;
  if (EV_USE_POLL  ) flags |= EVBACKEND_POLL;
  if (EV_USE_SELECT) flags |= EVBACKEND_SELECT;
  
  return flags;
}

unsigned int ev_recommended_backends (void)  
{
  unsigned int flags = ev_supported_backends ();

  /* kqueue is borked on everything but netbsd apparently */
  /* it usually doesn't work correctly on anything but sockets and pipes */
  flags &= ~EVBACKEND_KQUEUE;

  return flags;
}

unsigned int ev_embeddable_backends (void)  
{
  int flags = EVBACKEND_EPOLL | EVBACKEND_KQUEUE | EVBACKEND_PORT;

  /* epoll embeddability broken on all linux versions up to at least 2.6.23 */
  if (ev_linux_version () < 0x020620) /* disable it on linux < 2.6.32 */
    flags &= ~EVBACKEND_EPOLL;

  return flags;
}

unsigned int ev_backend (struct ev_loop *loop)  
{
  return loop->backend;
}

#if EV_FEATURE_API
unsigned int ev_iteration (struct ev_loop *loop)  
{
  return loop->loop_count;
}

unsigned int ev_depth (struct ev_loop *loop)  
{
  return loop->loop_depth;
}

void ev_set_io_collect_interval (struct ev_loop *loop, ev_tstamp interval)  
{
  loop->io_blocktime = interval;
}

void ev_set_timeout_collect_interval (struct ev_loop *loop, ev_tstamp interval)  
{
  loop->timeout_blocktime = interval;
}

void ev_set_userdata (struct ev_loop *loop, void *data)  
{
  loop->userdata = data;
}

void * ev_userdata (struct ev_loop *loop)  
{
  return loop->userdata;
}

void ev_set_invoke_pending_cb (struct ev_loop *loop, ev_loop_callback invoke_pending_cb)  
{
  loop->invoke_cb = invoke_pending_cb;
}

void ev_set_loop_release_cb (struct ev_loop *loop, void (*release)(struct ev_loop *loop)  , void (*acquire)(struct ev_loop *loop)  )  
{
  loop->release_cb = release;
  loop->acquire_cb = acquire;
}
#endif

/* initialise a loop structure, must be zero-initialised */
static void loop_init (struct ev_loop *loop, unsigned int flags)  
{
  if (!loop->backend) {
      loop->origflags = flags;

      /* pid check not overridable via env */
      if (flags & EVFLAG_FORKCHECK)
        loop->curpid = getpid ();

      if (!(flags & EVFLAG_NOENV) && !enable_secure() && getenv("LIBEV_FLAGS"))
        flags = atoi (getenv ("LIBEV_FLAGS"));

      loop->ev_rt_now          = ev_time ();
      loop->mn_now             = get_clock ();
      loop->now_floor          = loop->mn_now;
      loop->rtmn_diff          = loop->ev_rt_now - loop->mn_now;
#if EV_FEATURE_API
      loop->invoke_cb          = ev_invoke_pending;
#endif

      loop->io_blocktime       = 0.;
      loop->timeout_blocktime  = 0.;
      loop->backend            = 0;
      loop->backend_fd         = -1;
      loop->sig_pending        = 0;
#if EV_ASYNC_ENABLE
      loop->async_pending      = 0;
#endif
      loop->pipe_write_skipped = 0;
      loop->pipe_write_wanted  = 0;
      loop->evpipe [0]         = -1;
      loop->evpipe [1]         = -1;
#if EV_USE_INOTIFY
      loop->fs_fd              = flags & EVFLAG_NOINOTIFY ? -1 : -2;
#endif
#if EV_USE_SIGNALFD
      loop->sigfd              = flags & EVFLAG_SIGNALFD  ? -2 : -1;
#endif

      if (!(flags & EVBACKEND_MASK))
        flags |= ev_recommended_backends ();

#if EV_USE_EPOLL
      if (!loop->backend && (flags & EVBACKEND_EPOLL))
		loop->backend = epoll_init(loop, flags);
#endif
#if EV_USE_POLL
      if (!loop->backend && (flags & EVBACKEND_POLL))
		loop->backend = poll_init(loop, flags);
#endif
#if EV_USE_SELECT
      if (!loop->backend && (flags & EVBACKEND_SELECT))
		loop->backend = select_init (loop, flags);
#endif

      ev_prepare_init(&loop->pending_w, pendingcb);

#if EV_SIGNAL_ENABLE || EV_ASYNC_ENABLE
      ev_init (&loop->pipe_w, pipecb);
      ev_set_priority (&loop->pipe_w, EV_MAXPRI);
#endif
    }
}

/* free up a loop structure */
void ev_loop_destroy (struct ev_loop *loop)
{
  int i;

  /* mimic free (0) */
  if (!loop)
    return;

#if EV_CLEANUP_ENABLE
  /* queue cleanup watchers (and execute them) */
  if (expect_false (loop->cleanupcnt)) {
      queue_events (loop, (W *)loop->cleanups, loop->cleanupcnt, EV_CLEANUP);
      EV_INVOKE_PENDING;
  }
#endif

#if EV_CHILD_ENABLE
  if (ev_is_default_loop (loop) && ev_is_active (&childev))
    {
      ev_ref (loop); /* child watcher */
      ev_signal_stop (loop, &childev);
    }
#endif

  if (ev_is_active (&loop->pipe_w))
    {
      if (loop->evpipe [0] >= 0) close(loop->evpipe [0]);
      if (loop->evpipe [1] >= 0) close(loop->evpipe [1]);
    }

#if EV_USE_SIGNALFD
  if (ev_is_active (&loop->sigfd_w))
    close (loop->sigfd);
#endif

#if EV_USE_INOTIFY
  if (loop->fs_fd >= 0)
    close (loop->fs_fd);
#endif

  if (loop->backend_fd >= 0)
    close(loop->backend_fd);

#if EV_USE_EPOLL
  if (loop->backend == EVBACKEND_EPOLL)
	epoll_destroy(loop);
#endif
#if EV_USE_POLL
  if (loop->backend == EVBACKEND_POLL)
	poll_destroy   (loop);
#endif
#if EV_USE_SELECT
  if (loop->backend == EVBACKEND_SELECT)
	select_destroy (loop);
#endif

  for (i = NUMPRI; i--; )
    {
  	ev_free(loop->pendings[i]); loop->pendingcnt[i] = loop->pendingmax[i] = 0; loop->pendings[i] = 0;
#if EV_IDLE_ENABLE
	ev_free(loop->idles[i]); loop->idlecnt[i] = loop->idlemax[i] = 0; loop->idles[i] = 0;
#endif
    }

  ev_free (loop->anfds);
  loop->anfds = 0; 
  loop->anfdmax = 0;

  /* have to use the microsoft-never-gets-it-right macro */
  ev_free (loop->rfeeds); loop->rfeedcnt = loop->rfeedmax = 0; loop->rfeeds = 0;
  ev_free(loop->fdchanges); loop->fdchangecnt = loop->fdchangemax = 0; loop->fdchanges = 0;
  ev_free(loop->timers); loop->timercnt = loop->timermax = 0; loop->timers = 0;
#if EV_PERIODIC_ENABLE
  ev_free(loop->periodics); loop->periodiccnt = loop->periodicmax = 0; loop->periodics = 0;
#endif
#if EV_FORK_ENABLE
  ev_free(loop->forks); loop->forkcnt = loop->forkmax = 0; loop->forks = 0;
#endif
#if EV_CLEANUP_ENABLE
  ev_free(loop->cleanups); loop->cleanupcnt = loop->cleanupmax = 0; loop->cleanups = 0;
#endif
  ev_free(loop->prepares); loop->preparecnt = loop->preparemax = 0; loop->prepares = 0;
  ev_free(loop->checks); loop->checkcnt = loop->checkmax = 0; loop->checks = 0;
#if EV_ASYNC_ENABLE
  ev_free(loop->asyncs); loop->asynccnt = loop->asyncmax = 0; loop->asyncs = 0;
#endif

  loop->backend = 0;

  if (ev_is_default_loop (loop))
    ev_default_loop_ptr = 0;
  else
    ev_free (loop);
}

#if EV_USE_INOTIFY
static void infy_fork (struct ev_loop *loop);
#endif

static void loop_fork (struct ev_loop *loop)
{
#if EV_USE_EPOLL
  if (loop->backend == EVBACKEND_EPOLL)
	epoll_fork  (loop);
#endif

#if EV_USE_INOTIFY
  infy_fork (loop);
#endif

#if EV_SIGNAL_ENABLE || EV_ASYNC_ENABLE
  if (ev_is_active (&loop->pipe_w))
    {
      /* pipe_write_wanted must be false now, so modifying fd vars should be safe */

      ev_ref (loop);
      ev_io_stop (loop, &loop->pipe_w);

      if (loop->evpipe [0] >= 0)
        close(loop->evpipe [0]);

      evpipe_init (loop);
      /* iterate over everything, in case we missed something before */
      ev_feed_event (loop, &loop->pipe_w, EV_CUSTOM);
    }
#endif

  loop->postfork = 0;
}

struct ev_loop * ev_loop_new (unsigned int flags)  
{
  struct ev_loop *loop = (struct ev_loop *)ev_malloc (sizeof (struct ev_loop));

  memset (loop, 0, sizeof (struct ev_loop));
  loop_init (loop, flags);

  if (ev_backend (loop))
    return loop;

  ev_free (loop);
  return 0;
}

#if EV_VERIFY
static void verify_watcher (struct ev_loop *loop, W w)
{
  assert (("libev: watcher has invalid priority", ABSPRI (w) >= 0 && ABSPRI (w) < NUMPRI));

  if (w->pending)
    assert (("libev: pending watcher not on pending queue", loop->pendings [ABSPRI (w)][w->pending - 1].w == w));
}

static void verify_heap (struct ev_loop *loop, ANHE *heap, int N)
{
  int i;

  for (i = HEAP0; i < N + HEAP0; ++i)
    {
      assert (("libev: active index mismatch in heap", ev_active (ANHE_w (heap [i])) == i));
      assert (("libev: heap condition violated", i == HEAP0 || ANHE_at (heap [HPARENT (i)]) <= ANHE_at (heap [i])));
      assert (("libev: heap at cache mismatch", ANHE_at (heap [i]) == ev_at (ANHE_w (heap [i]))));

      verify_watcher (loop, (W)ANHE_w (heap [i]));
    }
}

static void array_verify (struct ev_loop *loop, W *ws, int cnt)
{
  while (cnt--)
    {
      assert (("libev: active index mismatch", ev_active (ws [cnt]) == cnt + 1));
      verify_watcher (loop, ws [cnt]);
    }
}
#endif

#if EV_FEATURE_API
void ev_verify (struct ev_loop *loop)  
{
#if EV_VERIFY
  int i;
  WL w, w2;

  assert (loop->activecnt >= -1);

  assert (loop->fdchangemax >= loop->fdchangecnt);
  for (i = 0; i < loop->fdchangecnt; ++i) {
    assert (("libev: negative fd in fdchanges", loop->fdchanges [i] >= 0));
  }

  assert (loop->anfdmax >= 0);
  for (i = 0; i < loop->anfdmax; ++i) {
      int j = 0;

      for (w = w2 = loop->anfds [i].head; w; w = w->next) {
          verify_watcher (loop, (W)w);

          if (j++ & 1) {
              assert (("libev: io watcher list contains a loop", w != w2));
              w2 = w2->next;
          }

          assert (("libev: inactive fd watcher on anfd list", ev_active (w) == 1));
          assert (("libev: fd mismatch between watcher and anfd", ((ev_io *)w)->fd == i));
        }
    }

  assert (loop->timermax >= loop->timercnt);
  verify_heap (loop, loop->timers, loop->timercnt);

#if EV_PERIODIC_ENABLE
  assert (loop->periodicmax >= loop->periodiccnt);
  verify_heap (loop, loop->periodics, loop->periodiccnt);
#endif

  for (i = NUMPRI; i--; ) {
      assert (loop->pendingmax[i] >= loop->pendingcnt[i]);
#if EV_IDLE_ENABLE
      assert (loop->idleall >= 0);
      assert (loop->idlemax[i] >= loop->idlecnt[i]);
      array_verify (loop, (W *)loop->idles[i], loop->idlecnt[i]);
#endif
    }

#if EV_FORK_ENABLE
  assert (loop->forkmax >= loop->forkcnt);
  array_verify (loop, (W *)loop->forks, loop->forkcnt);
#endif

#if EV_CLEANUP_ENABLE
  assert (loop->cleanupmax >= loop->cleanupcnt);
  array_verify (loop, (W *)loop->cleanups, loop->cleanupcnt);
#endif

#if EV_ASYNC_ENABLE
  assert (loop->asyncmax >= loop->asynccnt);
  array_verify (loop, (W *)loop->asyncs, loop->asynccnt);
#endif

#if EV_PREPARE_ENABLE
  assert (loop->preparemax >= loop->preparecnt);
  array_verify (loop, (W *)loop->prepares, loop->preparecnt);
#endif

#if EV_CHECK_ENABLE
  assert (loop->checkmax >= loop->checkcnt);
  array_verify (loop, (W *)loop->checks, loop->checkcnt);
#endif

#endif
}
#endif

struct ev_loop * ev_default_loop (unsigned int flags)  
{
  if (!ev_default_loop_ptr) {
      struct ev_loop *loop = ev_default_loop_ptr = &default_loop_struct;

      loop_init (loop, flags);

      if (ev_backend (loop)) {
#if EV_CHILD_ENABLE
          ev_signal_init (&childev, childcb, SIGCHLD);
          ev_set_priority (&childev, EV_MAXPRI);
          ev_signal_start (loop, &childev);
          ev_unref (loop); /* child watcher should not keep loop alive */
#endif
        } else {
			ev_default_loop_ptr = 0;
		}
    }

  return ev_default_loop_ptr;
}

void ev_loop_fork (struct ev_loop *loop)  
{
  loop->postfork = 1;
}

/*****************************************************************************/
void ev_invoke (struct ev_loop *loop, void *w, int revents)
{
  EV_CB_INVOKE ((W)w, revents);
}

unsigned int ev_pending_count (struct ev_loop *loop)  
{
  int pri;
  unsigned int count = 0;

  for (pri = NUMPRI; pri--; )
    count += loop->pendingcnt [pri];

  return count;
}

void ev_invoke_pending (struct ev_loop *loop)
{
  loop->pendingpri = NUMPRI;

  while (loop->pendingpri) { /* pendingpri possibly gets modified in the inner loop */
      --loop->pendingpri;

      while(loop->pendingcnt[loop->pendingpri]) {
          ANPENDING *p = loop->pendings [loop->pendingpri] + --loop->pendingcnt [loop->pendingpri];

          p->w->pending = 0;
          EV_CB_INVOKE (p->w, p->events);
          EV_FREQUENT_CHECK;
        }
    }
}

#if EV_IDLE_ENABLE
/* make idle watchers pending. this handles the "call-idle */
/* only when higher priorities are idle" logic */
static void idle_reify (struct ev_loop *loop)
{
  if (expect_false (loop->idleall)) {
      int pri;

      for (pri = NUMPRI; pri--; ) {
          if(loop->pendingcnt [pri])
            break;

          if (loop->idlecnt [pri]) {
              queue_events (loop, (W *)loop->idles [pri], loop->idlecnt [pri], EV_IDLE);
              break;
            }
        }
    }
}
#endif

/* make timers pending */
static void timers_reify (struct ev_loop *loop)
{
  EV_FREQUENT_CHECK;

  if (loop->timercnt && ANHE_at(loop->timers [HEAP0]) < loop->mn_now) {
      do
        {
          ev_timer *w = (ev_timer *)ANHE_w(loop->timers [HEAP0]);

          /*assert (("libev: inactive timer on timer heap detected", ev_is_active (w)));*/

          /* first reschedule or stop timer */
          if (w->repeat) {
              ev_at (w) += w->repeat;
              if (ev_at (w) < loop->mn_now)
                ev_at (w) = loop->mn_now;

              assert (("libev: negative ev_timer repeat value found while processing timers", w->repeat > 0.));

              ANHE_at_cache(loop->timers [HEAP0]);
              downheap(loop->timers, loop->timercnt, HEAP0);
          } else {
            ev_timer_stop (loop, w); /* nonrepeating: stop timer */
		  }

          EV_FREQUENT_CHECK;
          feed_reverse (loop, (W)w);
        }
      while (loop->timercnt && ANHE_at(loop->timers [HEAP0]) < loop->mn_now);

      feed_reverse_done (loop, EV_TIMER);
    }
}

#if EV_PERIODIC_ENABLE
static void periodic_recalc (struct ev_loop *loop, ev_periodic *w)
{
  ev_tstamp interval = w->interval > MIN_INTERVAL ? w->interval : MIN_INTERVAL;
  ev_tstamp at = w->offset + interval * ev_floor ((loop->ev_rt_now - w->offset) / interval);

  /* the above almost always errs on the low side */
  while (at <= loop->ev_rt_now) {
      ev_tstamp nat = at + w->interval;

      /* when resolution fails us, we use ev_rt_now */
      if (expect_false (nat == at)) {
          at = loop->ev_rt_now;
          break;
      }

      at = nat;
    }

  ev_at (w) = at;
}

/* make periodics pending */
static void periodics_reify (struct ev_loop *loop)
{
  EV_FREQUENT_CHECK;

  while(loop->periodiccnt && ANHE_at (loop->periodics [HEAP0]) < loop->ev_rt_now) {
      do{
          ev_periodic *w = (ev_periodic *)ANHE_w (loop->periodics [HEAP0]);

          /* first reschedule or stop timer */
          if (w->reschedule_cb){
              ev_at (w) = w->reschedule_cb (w, loop->ev_rt_now);

              assert (("libev: ev_periodic reschedule callback returned time in the past", ev_at (w) >= loop->ev_rt_now));

              ANHE_at_cache (loop->periodics [HEAP0]);
              downheap (loop->periodics, loop->periodiccnt, HEAP0);
            } else if (w->interval) {
              periodic_recalc (loop, w);
              ANHE_at_cache (loop->periodics [HEAP0]);
              downheap (loop->periodics, loop->periodiccnt, HEAP0);
            } else {
              ev_periodic_stop (loop, w); /* nonrepeating: stop timer */
			}

          EV_FREQUENT_CHECK;
          feed_reverse (loop, (W)w);
        }while (loop->periodiccnt && ANHE_at (loop->periodics [HEAP0]) < loop->ev_rt_now);

      feed_reverse_done (loop, EV_PERIODIC);
    }
}

/* simply recalculate all periodics */
/* TODO: maybe ensure that at least one event happens when jumping forward? */
static void periodics_reschedule (struct ev_loop *loop)
{
  int i;

  /* adjust periodics after time jump */
  for (i = HEAP0; i < loop->periodiccnt + HEAP0; ++i)
    {
      ev_periodic *w = (ev_periodic *)ANHE_w (loop->periodics [i]);

      if (w->reschedule_cb)
        ev_at (w) = w->reschedule_cb (w, loop->ev_rt_now);
      else if (w->interval)
        periodic_recalc (loop, w);

      ANHE_at_cache (loop->periodics [i]);
    }

  reheap (loop->periodics, loop->periodiccnt);
}
#endif

/* adjust all timers by a given offset */
static void timers_reschedule (struct ev_loop *loop, ev_tstamp adjust)
{
  int i;

  for (i = 0; i < loop->timercnt; ++i) {
      ANHE *he = loop->timers + i + HEAP0;
      ANHE_w (*he)->at += adjust;
      ANHE_at_cache (*he);
    }
}

/* fetch new monotonic and realtime times from the kernel */
/* also detect if there was a timejump, and act accordingly */
static void time_update (struct ev_loop *loop, ev_tstamp max_block)
{
	loop->ev_rt_now = ev_time (); 
	if (expect_false (loop->mn_now > loop->ev_rt_now || loop->ev_rt_now > loop->mn_now + max_block + MIN_TIMEJUMP)) {
		/* adjust timers. this is easy, as the offset is the same for all of them */
		timers_reschedule (loop, loop->ev_rt_now - loop->mn_now);
#if EV_PERIODIC_ENABLE
        periodics_reschedule (loop);
#endif
	}

	loop->mn_now = loop->ev_rt_now;
}

int ev_run (struct ev_loop *loop, int flags)
{
#if EV_FEATURE_API
  ++loop->loop_depth;
#endif

  assert (("libev: ev_loop recursion during release detected", loop->loop_done != EVBREAK_RECURSE));

  loop->loop_done = EVBREAK_CANCEL;

  EV_INVOKE_PENDING; /* in case we recurse, ensure ordering stays nice and clean */

  do
    {
#if EV_VERIFY >= 2
      ev_verify (loop);
#endif

      if (expect_false (loop->curpid)) /* penalise the forking check even more */
        if (expect_false (getpid () != loop->curpid))
          {
            loop->curpid= getpid ();
            loop->postfork = 1;
          }

#if EV_FORK_ENABLE
      /* we might have forked, so queue fork handlers */
      if (expect_false(loop->postfork))
        if (loop->forkcnt){
            queue_events (loop, (W *)loop->forks, loop->forkcnt, EV_FORK);
            EV_INVOKE_PENDING;
          }
#endif

#if EV_PREPARE_ENABLE
      /* queue prepare watchers (and execute them) */
      if (expect_false (loop->preparecnt))
        {
          queue_events (loop, (W *)loop->prepares, loop->preparecnt, EV_PREPARE);
          EV_INVOKE_PENDING;
        }
#endif

      if (expect_false(loop->loop_done))
        break;

      /* we might have forked, so reify kernel state if necessary */
      if (expect_false(loop->postfork))
        loop_fork (loop);

      /* update fd-related kernel structures */
      fd_reify (loop);

      /* calculate blocking time */
      {
        ev_tstamp waittime  = 0.;
        ev_tstamp sleeptime = 0.;

        /* remember old timestamp for io_blocktime calculation */
        ev_tstamp prev_mn_now = loop->mn_now;

        /* update time to cancel out callback processing overhead */
        time_update (loop, 1e100);

        /* from now on, we want a pipe-wake-up */
        loop->pipe_write_wanted = 1;

        ECB_MEMORY_FENCE; /* make sure pipe_write_wanted is visible before we check for potential skips */

        if (expect_true (!(flags & EVRUN_NOWAIT || loop->idleall || !loop->activecnt || loop->pipe_write_skipped))) {
            waittime = MAX_BLOCKTIME;

            if (loop->timercnt) {
                ev_tstamp to = ANHE_at(loop->timers [HEAP0]) - loop->mn_now;
                if (waittime > to) waittime = to;
            }

#if EV_PERIODIC_ENABLE
            if(loop->periodiccnt){
                ev_tstamp to = ANHE_at (loop->periodics [HEAP0]) - loop->ev_rt_now;
                if (waittime > to) waittime = to;
            }
#endif

            /* don't let timeouts decrease the waittime below timeout_blocktime */
            if(expect_false (waittime < loop->timeout_blocktime))
              waittime = loop->timeout_blocktime;

            /* at this point, we NEED to wait, so we have to ensure */
            /* to pass a minimum nonzero value to the backend */
            if (expect_false (waittime < loop->backend_mintime))
              waittime = loop->backend_mintime;

            /* extra check because io_blocktime is commonly 0 */
            if (expect_false (loop->io_blocktime))
              {
                sleeptime = loop->io_blocktime - (loop->mn_now - prev_mn_now);

                if (sleeptime > waittime - loop->backend_mintime)
                  sleeptime = waittime - loop->backend_mintime;

                if (expect_true (sleeptime > 0.))
                  {
                    ev_sleep (sleeptime);
                    waittime -= sleeptime;
                  }
              }
          }

#if EV_FEATURE_API
        ++loop->loop_count;
#endif
        assert ((loop->loop_done = EVBREAK_RECURSE, 1)); /* assert for side effect */
        loop->backend_poll(loop, waittime);
        assert ((loop->loop_done = EVBREAK_CANCEL, 1)); /* assert for side effect */

        loop->pipe_write_wanted = 0; /* just an optimisation, no fence needed */

        ECB_MEMORY_FENCE_ACQUIRE;
        if (loop->pipe_write_skipped)
          {
            assert (("libev: pipe_w not active, but pipe not written", ev_is_active (&loop->pipe_w)));
            ev_feed_event (loop, &loop->pipe_w, EV_CUSTOM);
          }


        /* update ev_rt_now, do magic */
        time_update (loop, waittime + sleeptime);
      }

      /* queue pending timers and reschedule them */
      timers_reify (loop); /* relative timers called last */
#if EV_PERIODIC_ENABLE
      periodics_reify (loop); /* absolute timers called first */
#endif

#if EV_IDLE_ENABLE
      /* queue idle watchers unless other events are pending */
      idle_reify (loop);
#endif

#if EV_CHECK_ENABLE
      /* queue check watchers, to be executed first */
      if (expect_false (loop->checkcnt))
        queue_events (loop, (W *)loop->checks, loop->checkcnt, EV_CHECK);
#endif

      EV_INVOKE_PENDING;
    }

  while (expect_true(loop->activecnt && !loop->loop_done &&!(flags & (EVRUN_ONCE | EVRUN_NOWAIT))));

  if (loop->loop_done == EVBREAK_ONE)
    loop->loop_done = EVBREAK_CANCEL;

#if EV_FEATURE_API
  --loop->loop_depth;
#endif

  return loop->activecnt;
}

void ev_break (struct ev_loop *loop, int how)  
{
  loop->loop_done = how;
}

void ev_ref (struct ev_loop *loop)  
{
  ++loop->activecnt;
}

void ev_unref (struct ev_loop *loop)  
{
  --loop->activecnt;
}

void ev_now_update (struct ev_loop *loop)  
{
  time_update (loop, 1e100);
}

void ev_suspend (struct ev_loop *loop)  
{
  ev_now_update (loop);
}

void ev_resume (struct ev_loop *loop)  
{
  ev_tstamp mn_prev = loop->mn_now;

  ev_now_update (loop);
  timers_reschedule (loop, loop->mn_now - mn_prev);
#if EV_PERIODIC_ENABLE
  /* TODO: really do this? */
  periodics_reschedule (loop);
#endif
}

/*****************************************************************************/
/* singly-linked list management, used when the expected list length is short */

static void wlist_add (WL *head, WL elem)
{
  elem->next = *head;
  *head = elem;
}

static void wlist_del (WL *head, WL elem)
{
  while (*head)
    {
      if (expect_true (*head == elem))
        {
          *head = elem->next;
          break;
        }

      head = &(*head)->next;
    }
}

/* internal, faster, version of ev_clear_pending */
static void clear_pending (struct ev_loop *loop, W w)
{
  if (w->pending) {
      loop->pendings [ABSPRI (w)][w->pending - 1].w = (W)&loop->pending_w;
      w->pending = 0;
  }
}

int ev_clear_pending (struct ev_loop *loop, void *w)  
{
  W w_ = (W)w;
  int pending = w_->pending;

  if (expect_true (pending)) {
      ANPENDING *p = loop->pendings [ABSPRI (w_)] + pending - 1;
      p->w = (W)&loop->pending_w;
      w_->pending = 0;
      return p->events;
    }
  else
    return 0;
}

static void pri_adjust (struct ev_loop *loop, W w)
{
  int pri = ev_priority (w);
  pri = pri < EV_MINPRI ? EV_MINPRI : pri;
  pri = pri > EV_MAXPRI ? EV_MAXPRI : pri;
  ev_set_priority (w, pri);
}

static void ev_start (struct ev_loop *loop, W w, int active)
{
  pri_adjust (loop, w);
  w->active = active;
  ev_ref (loop);
}

static void ev_stop (struct ev_loop *loop, W w)
{
  ev_unref (loop);
  w->active = 0;
}

/*****************************************************************************/
void ev_io_start (struct ev_loop *loop, ev_io *w)  
{
  int fd = w->fd;

  if (expect_false (ev_is_active (w)))
    return;

  assert (("libev: ev_io_start called with negative fd", fd >= 0));
  assert (("libev: ev_io_start called with illegal event mask", !(w->events & ~(EV__IOFDSET | EV_READ | EV_WRITE))));

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, 1);
  array_needsize (ANFD, loop->anfds, loop->anfdmax, fd + 1, array_init_zero);
  wlist_add (&loop->anfds[fd].head, (WL)w);

  /* common bug, apparently */
  assert (("libev: ev_io_start called with corrupted watcher", ((WL)w)->next != (WL)w));

  fd_change (loop, fd, w->events & EV__IOFDSET | EV_ANFD_REIFY);
  w->events &= ~EV__IOFDSET;

  EV_FREQUENT_CHECK;
}

void ev_io_stop (struct ev_loop *loop, ev_io *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  assert (("libev: ev_io_stop called with illegal fd (must stay constant after start!)", w->fd >= 0 && w->fd < loop->anfdmax));

  EV_FREQUENT_CHECK;

  wlist_del (&loop->anfds[w->fd].head, (WL)w);
  ev_stop (loop, (W)w);

  fd_change (loop, w->fd, EV_ANFD_REIFY);

  EV_FREQUENT_CHECK;
}

void ev_timer_start (struct ev_loop *loop, ev_timer *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  ev_at (w) += loop->mn_now;

  assert (("libev: ev_timer_start called with negative timer repeat value", w->repeat >= 0.));

  EV_FREQUENT_CHECK;

  ++loop->timercnt;
  ev_start (loop, (W)w, loop->timercnt + HEAP0 - 1);
  array_needsize (ANHE, loop->timers, loop->timermax, ev_active (w) + 1, EMPTY2);
  ANHE_w (loop->timers [ev_active (w)]) = (WT)w;
  ANHE_at_cache (loop->timers [ev_active (w)]);
  upheap (loop->timers, ev_active (w));

  EV_FREQUENT_CHECK;

}

void ev_timer_stop (struct ev_loop *loop, ev_timer *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    assert (("libev: internal timer heap corruption", ANHE_w (loop->timers [active]) == (WT)w));

    --loop->timercnt;

    if (expect_true (active < loop->timercnt + HEAP0))
      {
        loop->timers [active] = loop->timers [loop->timercnt + HEAP0];
        adjustheap (loop->timers, loop->timercnt, active);
      }
  }

  ev_at (w) -= loop->mn_now;

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}

void ev_timer_again (struct ev_loop *loop, ev_timer *w)  
{
  EV_FREQUENT_CHECK;

  clear_pending (loop, (W)w);

  if (ev_is_active (w))
    {
      if (w->repeat)
        {
          ev_at (w) = loop->mn_now + w->repeat;
          ANHE_at_cache (loop->timers [ev_active (w)]);
          adjustheap (loop->timers, loop->timercnt, ev_active (w));
        }
      else
        ev_timer_stop (loop, w);
    }
  else if (w->repeat)
    {
      ev_at (w) = w->repeat;
      ev_timer_start (loop, w);
    }

  EV_FREQUENT_CHECK;
}

ev_tstamp ev_timer_remaining (struct ev_loop *loop, ev_timer *w)  
{
  return ev_at (w) - (ev_is_active (w) ? loop->mn_now : 0.);
}

#if EV_PERIODIC_ENABLE
void ev_periodic_start (struct ev_loop *loop, ev_periodic *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  if (w->reschedule_cb)
    ev_at (w) = w->reschedule_cb (w, loop->ev_rt_now);
  else if (w->interval)
    {
      assert (("libev: ev_periodic_start called with negative interval value", w->interval >= 0.));
      periodic_recalc (loop, w);
    }
  else
    ev_at (w) = w->offset;

  EV_FREQUENT_CHECK;

  ++loop->periodiccnt;
  ev_start (loop, (W)w, loop->periodiccnt + HEAP0 - 1);
  array_needsize (ANHE, loop->periodics, loop->periodicmax, ev_active (w) + 1, EMPTY2);
  ANHE_w (loop->periodics [ev_active (w)]) = (WT)w;
  ANHE_at_cache (loop->periodics [ev_active (w)]);
  upheap (loop->periodics, ev_active (w));

  EV_FREQUENT_CHECK;

}

void ev_periodic_stop (struct ev_loop *loop, ev_periodic *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    assert (("libev: internal periodic heap corruption", ANHE_w (loop->periodics [active]) == (WT)w));

    --loop->periodiccnt;

    if (expect_true (active < loop->periodiccnt + HEAP0)) {
        loop->periodics [active] = loop->periodics [loop->periodiccnt + HEAP0];
        adjustheap (loop->periodics, loop->periodiccnt, active);
      }
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}

void ev_periodic_again (struct ev_loop *loop, ev_periodic *w)  
{
  /* TODO: use adjustheap and recalculation */
  ev_periodic_stop (loop, w);
  ev_periodic_start (loop, w);
}
#endif

#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#if EV_SIGNAL_ENABLE
void ev_signal_start (struct ev_loop *loop, ev_signal *w)  
{
  if (expect_false (ev_is_active (w))){
      return;
  }

  assert (("libev: ev_signal_start called with illegal signal number", w->signum > 0 && w->signum < EV_NSIG));

  assert (("libev: a signal must not be attached to two different loops",
           !signals [w->signum - 1].loop || signals [w->signum - 1].loop == loop));

  signals [w->signum - 1].loop = loop;
  ECB_MEMORY_FENCE_RELEASE;

  EV_FREQUENT_CHECK;

#if EV_USE_SIGNALFD
  if (loop->sigfd == -2) {
      loop->sigfd = signalfd (-1, &loop->sigfd_set, SFD_NONBLOCK | SFD_CLOEXEC);
      if (loop->sigfd < 0 && errno == EINVAL)
        loop->sigfd = signalfd (-1, &loop->sigfd_set, 0); /* retry without flags */

      if (loop->sigfd >= 0)
        {
          fd_intern (loop->sigfd); /* doing it twice will not hurt */

          sigemptyset (&loop->sigfd_set);

          ev_io_init (&loop->sigfd_w, sigfdcb, loop->sigfd, EV_READ);
          ev_set_priority (&loop->sigfd_w, EV_MAXPRI);
          ev_io_start (loop, &loop->sigfd_w);
          ev_unref (loop); /* signalfd watcher should not keep loop alive */
        }
    }

  if (loop->sigfd >= 0) {
      /* TODO: check .head */
      sigaddset (&loop->sigfd_set, w->signum);
      sigprocmask (SIG_BLOCK, &loop->sigfd_set, 0);

      signalfd (loop->sigfd, &loop->sigfd_set, 0);
    }
#endif

  ev_start (loop, (W)w, 1);
  wlist_add (&signals [w->signum - 1].head, (WL)w);

  if (!((WL)w)->next)
# if EV_USE_SIGNALFD
    if (loop->sigfd < 0) /*TODO*/
# endif
      {
        struct sigaction sa;

        evpipe_init (loop);

        sa.sa_handler = ev_sighandler;
        sigfillset (&sa.sa_mask);
        sa.sa_flags = SA_RESTART; /* if restarting works we save one iteration */
        sigaction (w->signum, &sa, 0);

        if (loop->origflags & EVFLAG_NOSIGMASK)
          {
            sigemptyset (&sa.sa_mask);
            sigaddset (&sa.sa_mask, w->signum);
            sigprocmask (SIG_UNBLOCK, &sa.sa_mask, 0);
          }
      }

  EV_FREQUENT_CHECK;
}

void ev_signal_stop (struct ev_loop *loop, ev_signal *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  wlist_del (&signals [w->signum - 1].head, (WL)w);
  ev_stop (loop, (W)w);

  if (!signals [w->signum - 1].head)
    {
      signals [w->signum - 1].loop = 0; /* unattach from signal */
#if EV_USE_SIGNALFD
      if (loop->sigfd >= 0)
        {
          sigset_t ss;

          sigemptyset (&ss);
          sigaddset (&ss, w->signum);
          sigdelset (&loop->sigfd_set, w->signum);

          signalfd (loop->sigfd, &loop->sigfd_set, 0);
          sigprocmask (SIG_UNBLOCK, &ss, 0);
        }
      else
#endif
        signal (w->signum, SIG_DFL);
    }

  EV_FREQUENT_CHECK;
}

#endif

#if EV_CHILD_ENABLE

void ev_child_start (struct ev_loop *loop, ev_child *w)  
{
  assert (("libev: child watchers are only supported in the default loop", loop == ev_default_loop_ptr));
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, 1);
  wlist_add (&childs [w->pid & ((EV_PID_HASHSIZE) - 1)], (WL)w);

  EV_FREQUENT_CHECK;
}

void ev_child_stop (struct ev_loop *loop, ev_child *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  wlist_del (&childs [w->pid & ((EV_PID_HASHSIZE) - 1)], (WL)w);
  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}

#endif

#if EV_STAT_ENABLE

#define DEF_STAT_INTERVAL  5.0074891
#define NFS_STAT_INTERVAL 30.1074891 /* for filesystems potentially failing inotify */
#define MIN_STAT_INTERVAL  0.1074891

static void stat_timer_cb (struct ev_loop *loop, ev_timer *w_, int revents);

#if EV_USE_INOTIFY

/* the * 2 is to allow for alignment padding, which for some reason is >> 8 */
# define EV_INOTIFY_BUFSIZE (sizeof (struct inotify_event) * 2 + NAME_MAX)

static void infy_add (struct ev_loop *loop, ev_stat *w)
{
  w->wd = inotify_add_watch (loop->fs_fd, w->path,
                             IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF | IN_MODIFY
                             | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO
                             | IN_DONT_FOLLOW | IN_MASK_ADD);

  if (w->wd >= 0)
    {
      struct statfs sfs;

      /* now local changes will be tracked by inotify, but remote changes won't */
      /* unless the filesystem is known to be local, we therefore still poll */
      /* also do poll on <2.6.25, but with normal frequency */

      if (!loop->fs_2625)
        w->timer.repeat = w->interval ? w->interval : DEF_STAT_INTERVAL;
      else if (!statfs (w->path, &sfs)
               && (sfs.f_type == 0x1373 /* devfs */
                   || sfs.f_type == 0x4006 /* fat */
                   || sfs.f_type == 0x4d44 /* msdos */
                   || sfs.f_type == 0xEF53 /* ext2/3 */
                   || sfs.f_type == 0x72b6 /* jffs2 */
                   || sfs.f_type == 0x858458f6 /* ramfs */
                   || sfs.f_type == 0x5346544e /* ntfs */
                   || sfs.f_type == 0x3153464a /* jfs */
                   || sfs.f_type == 0x9123683e /* btrfs */
                   || sfs.f_type == 0x52654973 /* reiser3 */
                   || sfs.f_type == 0x01021994 /* tmpfs */
                   || sfs.f_type == 0x58465342 /* xfs */))
        w->timer.repeat = 0.; /* filesystem is local, kernel new enough */
      else
        w->timer.repeat = w->interval ? w->interval : NFS_STAT_INTERVAL; /* remote, use reduced frequency */
    }
  else
    {
      /* can't use inotify, continue to stat */
      w->timer.repeat = w->interval ? w->interval : DEF_STAT_INTERVAL;

      /* if path is not there, monitor some parent directory for speedup hints */
      /* note that exceeding the hardcoded path limit is not a correctness issue, */
      /* but an efficiency issue only */
      if ((errno == ENOENT || errno == EACCES) && strlen (w->path) < 4096)
        {
          char path [4096];
          strcpy (path, w->path);

          do
            {
              int mask = IN_MASK_ADD | IN_DELETE_SELF | IN_MOVE_SELF
                       | (errno == EACCES ? IN_ATTRIB : IN_CREATE | IN_MOVED_TO);

              char *pend = strrchr (path, '/');

              if (!pend || pend == path)
                break;

              *pend = 0;
              w->wd = inotify_add_watch (loop->fs_fd, path, mask);
            }
          while (w->wd < 0 && (errno == ENOENT || errno == EACCES));
        }
    }

  if (w->wd >= 0) {
    wlist_add (&loop->fs_hash [w->wd & ((EV_INOTIFY_HASHSIZE) - 1)].head, (WL)w);
  }

  /* now re-arm timer, if required */
  if (ev_is_active (&w->timer)) ev_ref (loop);
  ev_timer_again (loop, &w->timer);
  if (ev_is_active (&w->timer)) ev_unref (loop);
}

static void infy_del (struct ev_loop *loop, ev_stat *w)
{
  int slot;
  int wd = w->wd;

  if (wd < 0)
    return;

  w->wd = -2;
  slot = wd & ((EV_INOTIFY_HASHSIZE) - 1);
  wlist_del (&loop->fs_hash [slot].head, (WL)w);

  /* remove this watcher, if others are watching it, they will rearm */
  inotify_rm_watch (loop->fs_fd, wd);
}

static void infy_wd (struct ev_loop *loop, int slot, int wd, struct inotify_event *ev)
{
  if (slot < 0)
    /* overflow, need to check for all hash slots */
    for (slot = 0; slot < (EV_INOTIFY_HASHSIZE); ++slot)
      infy_wd (loop, slot, wd, ev);
  else
    {
      WL w_;

      for (w_ = loop->fs_hash [slot & ((EV_INOTIFY_HASHSIZE) - 1)].head; w_; ) {
          ev_stat *w = (ev_stat *)w_;
          w_ = w_->next; /* lets us remove this watcher and all before it */

          if (w->wd == wd || wd == -1) {
              if (ev->mask & (IN_IGNORED | IN_UNMOUNT | IN_DELETE_SELF)) {
                  wlist_del (&loop->fs_hash [slot & ((EV_INOTIFY_HASHSIZE) - 1)].head, (WL)w);
                  w->wd = -1;
                  infy_add (loop, w); /* re-add, no matter what */
                }

              stat_timer_cb (loop, &w->timer, 0);
            }
        }
    }
}

static void infy_cb (struct ev_loop *loop, ev_io *w, int revents)
{
  char buf [EV_INOTIFY_BUFSIZE];
  int ofs;
  int len = read (loop->fs_fd, buf, sizeof (buf));

  for (ofs = 0; ofs < len; ) {
      struct inotify_event *ev = (struct inotify_event *)(buf + ofs);
      infy_wd (loop, ev->wd, ev->wd, ev);
      ofs += sizeof (struct inotify_event) + ev->len;
    }
}

static void ev_check_2625 (struct ev_loop *loop)
{
  /* kernels < 2.6.25 are borked
   * http://www.ussg.indiana.edu/hypermail/linux/kernel/0711.3/1208.html
   */
  if (ev_linux_version () < 0x020619)
    return;

  loop->fs_2625 = 1;
}

static int infy_newfd (void)
{
#if defined IN_CLOEXEC && defined IN_NONBLOCK
  int fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  if (fd >= 0)
    return fd;
#endif
  return inotify_init ();
}

static void infy_init (struct ev_loop *loop)
{
  if (loop->fs_fd != -2)
    return;

  loop->fs_fd = -1;

  ev_check_2625 (loop);

  loop->fs_fd = infy_newfd ();

  if (loop->fs_fd >= 0)
    {
      fd_intern (loop->fs_fd);
      ev_io_init (&loop->fs_w, infy_cb, loop->fs_fd, EV_READ);
      ev_set_priority (&loop->fs_w, EV_MAXPRI);
      ev_io_start (loop, &loop->fs_w);
      ev_unref (loop);
    }
}

static void infy_fork (struct ev_loop *loop)
{
  int slot;

  if (loop->fs_fd < 0)
    return;

  ev_ref (loop);
  ev_io_stop (loop, &loop->fs_w);
  close (loop->fs_fd);
  loop->fs_fd = infy_newfd ();

  if (loop->fs_fd >= 0) {
      fd_intern (loop->fs_fd);
      ev_io_set (&loop->fs_w, loop->fs_fd, EV_READ);
      ev_io_start (loop, &loop->fs_w);
      ev_unref (loop);
    }

  for (slot = 0; slot < (EV_INOTIFY_HASHSIZE); ++slot) {
      WL w_ = loop->fs_hash [slot].head;
      loop->fs_hash [slot].head = 0;

      while (w_) {
          ev_stat *w = (ev_stat *)w_;
          w_ = w_->next; /* lets us add this watcher */

          w->wd = -1;

          if (loop->fs_fd >= 0) {
            infy_add (loop, w); /* re-add, no matter what */
		  } else {
              w->timer.repeat = w->interval ? w->interval : DEF_STAT_INTERVAL;
              if (ev_is_active (&w->timer)) ev_ref (loop);
              ev_timer_again (loop, &w->timer);
              if (ev_is_active (&w->timer)) ev_unref (loop);
            }
        }
    }
}

#endif

# define EV_LSTAT(p,b) lstat (p, b)
void ev_stat_stat (struct ev_loop *loop, ev_stat *w)  
{
  if (lstat (w->path, &w->attr) < 0)
    w->attr.st_nlink = 0;
  else if (!w->attr.st_nlink)
    w->attr.st_nlink = 1;
}

static void stat_timer_cb (struct ev_loop *loop, ev_timer *w_, int revents)
{
  ev_stat *w = (ev_stat *)(((char *)w_) - offsetof (ev_stat, timer));

  ev_statdata prev = w->attr;
  ev_stat_stat (loop, w);

  /* memcmp doesn't work on netbsd, they.... do stuff to their struct stat */
  if (
    prev.st_dev      != w->attr.st_dev
    || prev.st_ino   != w->attr.st_ino
    || prev.st_mode  != w->attr.st_mode
    || prev.st_nlink != w->attr.st_nlink
    || prev.st_uid   != w->attr.st_uid
    || prev.st_gid   != w->attr.st_gid
    || prev.st_rdev  != w->attr.st_rdev
    || prev.st_size  != w->attr.st_size
    || prev.st_atime != w->attr.st_atime
    || prev.st_mtime != w->attr.st_mtime
    || prev.st_ctime != w->attr.st_ctime
  ) {
      /* we only update w->prev on actual differences */
      /* in case we test more often than invoke the callback, */
      /* to ensure that prev is always different to attr */
      w->prev = prev;

      #if EV_USE_INOTIFY
        if (loop->fs_fd >= 0)
          {
            infy_del (loop, w);
            infy_add (loop, w);
            ev_stat_stat (loop, w); /* avoid race... */
          }
      #endif

      ev_feed_event (loop, w, EV_STAT);
    }
}

void ev_stat_start (struct ev_loop *loop, ev_stat *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  ev_stat_stat (loop, w);

  if (w->interval < MIN_STAT_INTERVAL && w->interval)
    w->interval = MIN_STAT_INTERVAL;

  ev_timer_init (&w->timer, stat_timer_cb, 0., w->interval ? w->interval : DEF_STAT_INTERVAL);
  ev_set_priority (&w->timer, ev_priority (w));

#if EV_USE_INOTIFY
  infy_init (loop);

  if (loop->fs_fd >= 0)
    infy_add (loop, w);
  else
#endif
    {
      ev_timer_again (loop, &w->timer);
      ev_unref (loop);
    }

  ev_start (loop, (W)w, 1);

  EV_FREQUENT_CHECK;
}

void ev_stat_stop (struct ev_loop *loop, ev_stat *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

#if EV_USE_INOTIFY
  infy_del (loop, w);
#endif

  if (ev_is_active (&w->timer))
    {
      ev_ref (loop);
      ev_timer_stop (loop, &w->timer);
    }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_IDLE_ENABLE
void ev_idle_start (struct ev_loop *loop, ev_idle *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  pri_adjust (loop, (W)w);

  EV_FREQUENT_CHECK;

  {
    int active = ++loop->idlecnt [ABSPRI (w)];

    ++loop->idleall;
    ev_start (loop, (W)w, active);

    array_needsize (ev_idle *, loop->idles [ABSPRI (w)], loop->idlemax [ABSPRI (w)], active, EMPTY2);
    loop->idles [ABSPRI (w)][active - 1] = w;
  }

  EV_FREQUENT_CHECK;
}

void ev_idle_stop (struct ev_loop *loop, ev_idle *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    loop->idles [ABSPRI (w)][active - 1] = loop->idles [ABSPRI (w)][--loop->idlecnt [ABSPRI (w)]];
    ev_active (loop->idles [ABSPRI (w)][active - 1]) = active;

    ev_stop (loop, (W)w);
    --loop->idleall;
  }

  EV_FREQUENT_CHECK;
}
#endif

#if EV_PREPARE_ENABLE
void ev_prepare_start (struct ev_loop *loop, ev_prepare *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++loop->preparecnt);
  array_needsize (ev_prepare *, loop->prepares, loop->preparemax, loop->preparecnt, EMPTY2);
  loop->prepares [loop->preparecnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void ev_prepare_stop (struct ev_loop *loop, ev_prepare *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    loop->prepares [active - 1] = loop->prepares [--loop->preparecnt];
    ev_active (loop->prepares [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_CHECK_ENABLE
void ev_check_start (struct ev_loop *loop, ev_check *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++loop->checkcnt);
  array_needsize (ev_check *, loop->checks, loop->checkmax, loop->checkcnt, EMPTY2);
  loop->checks [loop->checkcnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void ev_check_stop (struct ev_loop *loop, ev_check *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    loop->checks [active - 1] = loop->checks [--loop->checkcnt];
    ev_active (loop->checks [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_EMBED_ENABLE
void ev_embed_sweep (struct ev_loop *loop, ev_embed *w)  
{
  ev_run (w->other, EVRUN_NOWAIT);
}

static void embed_io_cb (struct ev_loop *loop, ev_io *io, int revents)
{
  ev_embed *w = (ev_embed *)(((char *)io) - offsetof (ev_embed, io));

  if (ev_cb (w))
    ev_feed_event (loop, (W)w, EV_EMBED);
  else
    ev_run (w->other, EVRUN_NOWAIT);
}

static void embed_prepare_cb (struct ev_loop *loop, ev_prepare *prepare, int revents)
{
  ev_embed *w = (ev_embed *)(((char *)prepare) - offsetof (ev_embed, prepare));

  {
    struct ev_loop *loop = w->other;

    while (loop->fdchangecnt) {
        fd_reify (loop);
        ev_run (loop, EVRUN_NOWAIT);
      }
  }
}

static void embed_fork_cb (struct ev_loop *loop, ev_fork *fork_w, int revents)
{
  ev_embed *w = (ev_embed *)(((char *)fork_w) - offsetof (ev_embed, fork));

  ev_embed_stop (loop, w);

  {
    struct ev_loop *loop = w->other;

    ev_loop_fork (loop);
    ev_run (loop, EVRUN_NOWAIT);
  }

  ev_embed_start (loop, w);
}

void ev_embed_start (struct ev_loop *loop, ev_embed *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  {
    struct ev_loop *loop = w->other;
    assert (("libev: loop to be embedded is not embeddable", loop->backend & ev_embeddable_backends ()));
    ev_io_init (&w->io, embed_io_cb, loop->backend_fd, EV_READ);
  }

  EV_FREQUENT_CHECK;

  ev_set_priority (&w->io, ev_priority (w));
  ev_io_start (loop, &w->io);

  ev_prepare_init (&w->prepare, embed_prepare_cb);
  ev_set_priority (&w->prepare, EV_MINPRI);
  ev_prepare_start (loop, &w->prepare);

  ev_fork_init (&w->fork, embed_fork_cb);
  ev_fork_start (loop, &w->fork);

  /*ev_idle_init (&w->idle, e,bed_idle_cb);*/

  ev_start (loop, (W)w, 1);

  EV_FREQUENT_CHECK;
}

void ev_embed_stop (struct ev_loop *loop, ev_embed *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_io_stop      (loop, &w->io);
  ev_prepare_stop (loop, &w->prepare);
  ev_fork_stop    (loop, &w->fork);

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_FORK_ENABLE
void ev_fork_start (struct ev_loop *loop, ev_fork *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++loop->forkcnt);
  array_needsize (ev_fork *, loop->forks, loop->forkmax, loop->forkcnt, EMPTY2);
  loop->forks [loop->forkcnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void ev_fork_stop (struct ev_loop *loop, ev_fork *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    loop->forks [active - 1] = loop->forks [--loop->forkcnt];
    ev_active (loop->forks [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_CLEANUP_ENABLE
void ev_cleanup_start (struct ev_loop *loop, ev_cleanup *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++loop->cleanupcnt);
  array_needsize (ev_cleanup *, loop->cleanups, loop->cleanupmax, loop->cleanupcnt, EMPTY2);
  loop->cleanups [loop->cleanupcnt - 1] = w;

  /* cleanup watchers should never keep a refcount on the loop */
  ev_unref (loop);
  EV_FREQUENT_CHECK;
}

void ev_cleanup_stop (struct ev_loop *loop, ev_cleanup *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;
  ev_ref (loop);

  {
    int active = ev_active (w);

    loop->cleanups [active - 1] = loop->cleanups [--loop->cleanupcnt];
    ev_active (loop->cleanups [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_ASYNC_ENABLE
void ev_async_start (struct ev_loop *loop, ev_async *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  w->sent = 0;

  evpipe_init (loop);

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++loop->asynccnt);
  array_needsize (ev_async *, loop->asyncs, loop->asyncmax, loop->asynccnt, EMPTY2);
  loop->asyncs [loop->asynccnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void ev_async_stop (struct ev_loop *loop, ev_async *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    loop->asyncs [active - 1] = loop->asyncs [--loop->asynccnt];
    ev_active (loop->asyncs [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}

void ev_async_send (struct ev_loop *loop, ev_async *w)  
{
  w->sent = 1;
  evpipe_write (loop, &loop->async_pending);
}
#endif

/*****************************************************************************/
struct ev_once
{
  ev_io io;
  ev_timer to;
  void (*cb)(int revents, void *arg);
  void *arg;
};

static void once_cb (struct ev_loop *loop, struct ev_once *once, int revents)
{
  void (*cb)(int revents, void *arg) = once->cb;
  void *arg = once->arg;

  ev_io_stop    (loop, &once->io);
  ev_timer_stop (loop, &once->to);
  ev_free (once);

  cb (revents, arg);
}

static void once_cb_io (struct ev_loop *loop, ev_io *w, int revents)
{
  struct ev_once *once = (struct ev_once *)(((char *)w) - offsetof (struct ev_once, io));

  once_cb (loop, once, revents | ev_clear_pending (loop, &once->to));
}

static void once_cb_to (struct ev_loop *loop, ev_timer *w, int revents)
{
  struct ev_once *once = (struct ev_once *)(((char *)w) - offsetof (struct ev_once, to));

  once_cb (loop, once, revents | ev_clear_pending (loop, &once->io));
}

void ev_once (struct ev_loop *loop, int fd, int events, ev_tstamp timeout, void (*cb)(int revents, void *arg), void *arg)  
{
  struct ev_once *once = (struct ev_once *)ev_malloc (sizeof (struct ev_once));

  if (expect_false (!once))
    {
      cb (EV_ERROR | EV_READ | EV_WRITE | EV_TIMER, arg);
      return;
    }

  once->cb  = cb;
  once->arg = arg;

  ev_init (&once->io, once_cb_io);
  if (fd >= 0)
    {
      ev_io_set (&once->io, fd, events);
      ev_io_start (loop, &once->io);
    }

  ev_init (&once->to, once_cb_to);
  if (timeout >= 0.)
    {
      ev_timer_set (&once->to, timeout, 0.);
      ev_timer_start (loop, &once->to);
    }
}
