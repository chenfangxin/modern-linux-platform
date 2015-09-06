/* this big block deduces configuration from config.h */
#include "config.h"

#ifndef EV_USE_NANOSLEEP
#define EV_USE_NANOSLEEP EV_FEATURE_OS
#endif

# if HAVE_SELECT && HAVE_SYS_SELECT_H
#  ifndef EV_USE_SELECT
#   define EV_USE_SELECT EV_FEATURE_BACKENDS
#  endif
# else
#  undef EV_USE_SELECT
#  define EV_USE_SELECT 0
# endif

# if HAVE_POLL && HAVE_POLL_H
#  ifndef EV_USE_POLL
#   define EV_USE_POLL EV_FEATURE_BACKENDS
#  endif
# else
#  undef EV_USE_POLL
#  define EV_USE_POLL 0
# endif
   
# if HAVE_EPOLL_CTL && HAVE_SYS_EPOLL_H
#  ifndef EV_USE_EPOLL
#   define EV_USE_EPOLL EV_FEATURE_BACKENDS
#  endif
# else
#  undef EV_USE_EPOLL
#  define EV_USE_EPOLL 0
# endif
   
# if HAVE_INOTIFY_INIT && HAVE_SYS_INOTIFY_H
#  ifndef EV_USE_INOTIFY
#   define EV_USE_INOTIFY EV_FEATURE_OS
#  endif
# else
#  undef EV_USE_INOTIFY
#  define EV_USE_INOTIFY 0
# endif

# if HAVE_SIGNALFD && HAVE_SYS_SIGNALFD_H
#  ifndef EV_USE_SIGNALFD
#   define EV_USE_SIGNALFD EV_FEATURE_OS
#  endif
# else
#  undef EV_USE_SIGNALFD
#  define EV_USE_SIGNALFD 0
# endif

# if HAVE_EVENTFD
#  ifndef EV_USE_EVENTFD
#   define EV_USE_EVENTFD EV_FEATURE_OS
#  endif
# else
#  undef EV_USE_EVENTFD
#  define EV_USE_EVENTFD 0
# endif
 
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

#ifndef EV_USE_EVENTFD
# if __linux && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 7))
#  define EV_USE_EVENTFD EV_FEATURE_OS
# else
#  define EV_USE_EVENTFD 0
# endif
#endif

#ifndef EV_USE_SIGNALFD
# if __linux && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 7))
#  define EV_USE_SIGNALFD EV_FEATURE_OS
# else
#  define EV_USE_SIGNALFD 0
# endif
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
// #define MIN_INTERVAL  0.00000095367431640625 /* 1/2**20, good till 2200 */

#define MIN_TIMEJUMP  1. /* minimum timejump that gets detected (if monotonic clock available) */
#define MAX_BLOCKTIME 59.743 /* never wait longer than this time (to detect time jumps) */

#define EV_TV_SET(tv,t) do { tv.tv_sec = (long)t; tv.tv_usec = (long)((t - tv.tv_sec) * 1e6); } while (0)
#define EV_TS_SET(ts,t) do { ts.tv_sec = (long)t; ts.tv_nsec = (long)((t - ts.tv_sec) * 1e9); } while (0)

/* the following is ecb.h embedded into libev - use update_ev_c to update from an external copy */
/* ECB.H BEGIN */
/*
 * libecb - http://software.schmorp.de/pkg/libecb
 *
 * Copyright (©) 2009-2015 Marc Alexander Lehmann <libecb@schmorp.de>
 * Copyright (©) 2011 Emanuele Giaquinta
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

#ifndef ECB_H
#define ECB_H

/* 16 bits major, 16 bits minor */
#define ECB_VERSION 0x00010004

#include <inttypes.h>
#if UINTMAX_MAX > 0xffffffffU
  #define ECB_PTRSIZE 8
#else
  #define ECB_PTRSIZE 4
#endif

#define ECB_GCC_AMD64 (__amd64 || __amd64__ || __x86_64 || __x86_64__)
#define ECB_MSVC_AMD64 (_M_AMD64 || _M_X64)

/* work around x32 idiocy by defining proper macros */
#if ECB_GCC_AMD64 || ECB_MSVC_AMD64
  #if _ILP32
    #define ECB_AMD64_X32 1
  #else
    #define ECB_AMD64 1
  #endif
#endif

/* many compilers define _GNUC_ to some versions but then only implement
 * what their idiot authors think are the "more important" extensions,
 * causing enormous grief in return for some better fake benchmark numbers.
 * or so.
 * we try to detect these and simply assume they are not gcc - if they have
 * an issue with that they should have done it right in the first place.
 */
#define ECB_GCC_VERSION(major,minor) (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))

#define ECB_CLANG_VERSION(major,minor) (__clang_major__ > (major) || (__clang_major__ == (major) && __clang_minor__ >= (minor)))

#if __clang__ && defined __has_builtin
  #define ECB_CLANG_BUILTIN(x) __has_builtin (x)
#else
  #define ECB_CLANG_BUILTIN(x) 0
#endif

#if __clang__ && defined __has_extension
  #define ECB_CLANG_EXTENSION(x) __has_extension (x)
#else
  #define ECB_CLANG_EXTENSION(x) 0
#endif

#define ECB_CPP   (__cplusplus+0)
#define ECB_CPP11 (__cplusplus >= 201103L)

#define ECB_C            1
#define ECB_STDC_VERSION __STDC_VERSION__

#define ECB_C99   (ECB_STDC_VERSION >= 199901L)
#define ECB_C11   (ECB_STDC_VERSION >= 201112L)

#define ECB_EXTERN_C extern
#define ECB_EXTERN_C_BEG
#define ECB_EXTERN_C_END

/*****************************************************************************/
/* http://www-01.ibm.com/support/knowledgecenter/SSGH3R_13.1.0/com.ibm.xlcpp131.aix.doc/compiler_ref/compiler_builtins.html */
#ifndef ECB_MEMORY_FENCE
  #if ECB_GCC_VERSION(2,5) || defined __INTEL_COMPILER || (__llvm__ && __GNUC__) || __SUNPRO_C >= 0x5110 || __SUNPRO_CC >= 0x5110
    #if __i386 || __i386__
      #define ECB_MEMORY_FENCE         __asm__ __volatile__ ("lock; orb $0, -1(%%esp)" : : : "memory")
      #define ECB_MEMORY_FENCE_ACQUIRE __asm__ __volatile__ (""                        : : : "memory")
      #define ECB_MEMORY_FENCE_RELEASE __asm__ __volatile__ ("")
    #elif ECB_GCC_AMD64
      #define ECB_MEMORY_FENCE         __asm__ __volatile__ ("mfence"   : : : "memory")
      #define ECB_MEMORY_FENCE_ACQUIRE __asm__ __volatile__ (""         : : : "memory")
      #define ECB_MEMORY_FENCE_RELEASE __asm__ __volatile__ ("")
	#endif
  #endif
#endif

#ifndef ECB_MEMORY_FENCE
    /* see comment below (stdatomic.h) about the C11 memory model. */
    #define ECB_MEMORY_FENCE         __atomic_thread_fence (__ATOMIC_SEQ_CST)
    #define ECB_MEMORY_FENCE_ACQUIRE __atomic_thread_fence (__ATOMIC_ACQUIRE)
    #define ECB_MEMORY_FENCE_RELEASE __atomic_thread_fence (__ATOMIC_RELEASE)
#endif

#ifndef ECB_MEMORY_FENCE
  #if ECB_C11 && !defined __STDC_NO_ATOMICS__
    /* we assume that these memory fences work on all variables/all memory accesses, */
    /* not just C11 atomics and atomic accesses */
    #include <stdatomic.h>
    /* Unfortunately, neither gcc 4.7 nor clang 3.1 generate any instructions for */
    /* any fence other than seq_cst, which isn't very efficient for us. */
    /* Why that is, we don't know - either the C11 memory model is quite useless */
    /* for most usages, or gcc and clang have a bug */
    /* I *currently* lean towards the latter, and inefficiently implement */
    /* all three of ecb's fences as a seq_cst fence */
    /* Update, gcc-4.8 generates mfence for all c++ fences, but nothing */
    /* for all __atomic_thread_fence's except seq_cst */
    #define ECB_MEMORY_FENCE         atomic_thread_fence (memory_order_seq_cst)
  #endif
#endif

#ifndef ECB_MEMORY_FENCE
  #if !ECB_AVOID_PTHREADS
    /*
     * if you get undefined symbol references to pthread_mutex_lock,
     * or failure to find pthread.h, then you should implement
     * the ECB_MEMORY_FENCE operations for your cpu/compiler
     * OR provide pthread.h and link against the posix thread library
     * of your system.
     */
    #include <pthread.h>
    #define ECB_NEEDS_PTHREADS 1
    #define ECB_MEMORY_FENCE_NEEDS_PTHREADS 1

    static pthread_mutex_t ecb_mf_lock = PTHREAD_MUTEX_INITIALIZER;
    #define ECB_MEMORY_FENCE do { pthread_mutex_lock (&ecb_mf_lock); pthread_mutex_unlock (&ecb_mf_lock); } while (0)
  #endif
#endif

#if !defined ECB_MEMORY_FENCE_ACQUIRE && defined ECB_MEMORY_FENCE
  #define ECB_MEMORY_FENCE_ACQUIRE ECB_MEMORY_FENCE
#endif

#if !defined ECB_MEMORY_FENCE_RELEASE && defined ECB_MEMORY_FENCE
  #define ECB_MEMORY_FENCE_RELEASE ECB_MEMORY_FENCE
#endif

/*****************************************************************************/

#define ECB_CONCAT_(a, b) a ## b
#define ECB_CONCAT(a, b) ECB_CONCAT_(a, b)
#define ECB_STRINGIFY_(a) # a
#define ECB_STRINGIFY(a) ECB_STRINGIFY_(a)
#define ECB_STRINGIFY_EXPR(expr) ((expr), ECB_STRINGIFY_ (expr))

#define ecb_attribute(attrlist)        __attribute__ (attrlist)

#if ECB_GCC_VERSION(3,1) || ECB_CLANG_BUILTIN(__builtin_constant_p)
  #define ecb_is_constant(expr)          __builtin_constant_p (expr)
#else
  /* possible C11 impl for integral types
  typedef struct ecb_is_constant_struct ecb_is_constant_struct;
  #define ecb_is_constant(expr)          _Generic ((1 ? (struct ecb_is_constant_struct *)0 : (void *)((expr) - (expr)), ecb_is_constant_struct *: 0, default: 1)) */

  #define ecb_is_constant(expr)          0
#endif

#define ecb_expect(expr,value)         __builtin_expect ((expr),(value))

/* put around conditional expressions if you are very sure that the  */
/* expression is mostly true or mostly false. note that these return */
/* booleans, not the expression.                                     */
#define ecb_expect_false(expr) ecb_expect (!!(expr), 0)
#define ecb_expect_true(expr)  ecb_expect (!!(expr), 1)
/* for compatibility to the rest of the world */
#define ecb_likely(expr)   ecb_expect_true  (expr)
#define ecb_unlikely(expr) ecb_expect_false (expr)

#endif

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

static unsigned int 
ev_linux_version (void)
{
#ifdef __linux
  unsigned int v = 0;
  struct utsname buf;
  int i;
  char *p = buf.release;

  if (uname (&buf))
    return 0;

  for (i = 3+1; --i; )
    {
      unsigned int c = 0;

      for (;;)
        {
          if (*p >= '0' && *p <= '9')
            c = c * 10 + *p++ - '0';
          else
            {
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
static void 
ev_printerr (const char *msg)
{
  write (STDERR_FILENO, msg, strlen (msg));
}
#endif

static void (*syserr_cb)(const char *msg)  ;

void ev_set_syserr_cb (void (*cb)(const char *msg)  )  
{
  syserr_cb = cb;
}

static void 
ev_syserr (const char *msg)
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

static void *
ev_realloc_emul (void *ptr, long size)  
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

void 
ev_set_allocator (void *(*cb)(void *ptr, long size)  )  
{
  alloc = cb;
}

static void *
ev_realloc (void *ptr, long size)
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
typedef struct
{
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
typedef struct
{
  W w;
  int events; /* the pending event set for the given watcher */
} ANPENDING;

#if EV_USE_INOTIFY
/* hash table entry per inotify-id */
typedef struct
{
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
    #define ev_rt_now ((loop)->ev_rt_now)
    #define VAR(name,decl) decl;
      #include "ev_vars.h"
    #undef VAR
};
#include "ev_wrap.h"

static struct ev_loop default_loop_struct;
struct ev_loop *ev_default_loop_ptr = 0; /* needs to be initialised to make it a definition despite extern */

#if EV_FEATURE_API
# define EV_RELEASE_CB if (expect_false (release_cb)) release_cb (loop)
# define EV_ACQUIRE_CB if (expect_false (acquire_cb)) acquire_cb (loop)
# define EV_INVOKE_PENDING invoke_cb (loop)
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
  return ev_rt_now;
}

void
ev_sleep (ev_tstamp delay)  
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
  if (expect_false ((cnt) > (cur)))				\
    {								\
      int ocur_ = (cur);					\
      (base) = (type *)array_realloc				\
         (sizeof (type), (base), &(cur), (cnt));		\
      init ((base) + (ocur_), (cur) - ocur_);			\
    }


#define array_free(stem, idx) \
  ev_free (stem ## s idx); stem ## cnt idx = stem ## max idx = 0; stem ## s idx = 0

/*****************************************************************************/

/* dummy callback for pending events */
static void pendingcb (struct ev_loop *loop, ev_prepare *w, int revents)
{

}

void ev_feed_event (struct ev_loop *loop, void *w, int revents)  
{
  W w_ = (W)w;
  int pri = ABSPRI (w_);

  if (expect_false (w_->pending))
    pendings [pri][w_->pending - 1].events |= revents;
  else
    {
      w_->pending = ++pendingcnt [pri];
      array_needsize (ANPENDING, pendings [pri], pendingmax [pri], w_->pending, EMPTY2);
      pendings [pri][w_->pending - 1].w      = w_;
      pendings [pri][w_->pending - 1].events = revents;
    }

  pendingpri = NUMPRI - 1;
}

static void
feed_reverse (struct ev_loop *loop, W w)
{
  array_needsize (W, rfeeds, rfeedmax, rfeedcnt + 1, EMPTY2);
  rfeeds [rfeedcnt++] = w;
}

static void feed_reverse_done (struct ev_loop *loop, int revents)
{
  do{
    ev_feed_event (loop, rfeeds [--rfeedcnt], revents);
  }while (rfeedcnt);
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
  ANFD *anfd = anfds + fd;
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
  ANFD *anfd = anfds + fd;

  if (expect_true (!anfd->reify))
    fd_event_nocheck (loop, fd, revents);
}

void ev_feed_fd_event (struct ev_loop *loop, int fd, int revents)  
{
  if (fd >= 0 && fd < anfdmax)
    fd_event_nocheck (loop, fd, revents);
}

/* make sure the external fd watch events are in-sync */
/* with the kernel/libev internal state */
static void fd_reify (struct ev_loop *loop)
{
  int i;

  for (i = 0; i < fdchangecnt; ++i)
    {
      int fd = fdchanges [i];
      ANFD *anfd = anfds + fd;
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
        backend_modify (loop, fd, o_events, anfd->events);
    }

  fdchangecnt = 0;
}

/* something about the given fd changed */
static void fd_change (struct ev_loop *loop, int fd, int flags)
{
  unsigned char reify = anfds [fd].reify;
  anfds [fd].reify |= flags;

  if (expect_true (!reify))
    {
      ++fdchangecnt;
      array_needsize (int, fdchanges, fdchangemax, fdchangecnt, EMPTY2);
      fdchanges [fdchangecnt - 1] = fd;
    }
}

/* the given fd is invalid/unusable, so make sure it doesn't hurt us anymore */
static void fd_kill (struct ev_loop *loop, int fd)
{
  ev_io *w;

  while ((w = (ev_io *)anfds [fd].head))
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

  for (fd = 0; fd < anfdmax; ++fd)
    if (anfds [fd].events)
      if (!fd_valid (fd) && errno == EBADF)
        fd_kill (loop, fd);
}

/* called on ENOMEM in select/poll to kill some fds and retry */
static void fd_enomem (struct ev_loop *loop)
{
  int fd;

  for (fd = anfdmax; fd--; )
    if (anfds [fd].events)
      {
        fd_kill (loop, fd);
        break;
      }
}

/* usually called after fork if backend needs to re-arm all fds from scratch */
static void fd_rearm_all (struct ev_loop *loop)
{
  int fd;

  for (fd = 0; fd < anfdmax; ++fd)
    if (anfds [fd].events)
      {
        anfds [fd].events = 0;
        anfds [fd].emask  = 0;
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
  if (!ev_is_active (&pipe_w))
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

      evpipe [0] = fds [0];

      if (evpipe [1] < 0){
        evpipe [1] = fds [1]; /* first call, set write fd */
	  }else {
          /* on subsequent calls, do not change evpipe [1] */
          /* so that evpipe_write can always rely on its value. */
          /* this branch does not do anything sensible on windows, */
          /* so must not be executed on windows */

          dup2 (fds [1], evpipe [1]);
          close (fds [1]);
      }

      fd_intern (evpipe [1]);

      ev_io_set (&pipe_w, evpipe [0] < 0 ? evpipe [1] : evpipe [0], EV_READ);
      ev_io_start (loop, &pipe_w);
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

  pipe_write_skipped = 1;

  ECB_MEMORY_FENCE; /* make sure pipe_write_skipped is visible before we check pipe_write_wanted */

  if (pipe_write_wanted)
    {
      int old_errno;

      pipe_write_skipped = 0;
      ECB_MEMORY_FENCE_RELEASE;

      old_errno = errno; /* save errno because write will clobber it */

#if EV_USE_EVENTFD
      if (evpipe [0] < 0)
        {
          uint64_t counter = 1;
          write (evpipe [1], &counter, sizeof (uint64_t));
        }
      else
#endif
        {
          write (evpipe [1], &(evpipe [1]), 1);
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
      if (evpipe [0] < 0)
        {
          uint64_t counter;
          read (evpipe [1], &counter, sizeof (uint64_t));
        }
      else
#endif
        {
          char dummy[4];
          read (evpipe [0], &dummy, sizeof (dummy));
        }
    }

  pipe_write_skipped = 0;

  ECB_MEMORY_FENCE; /* push out skipped, acquire flags */

#if EV_SIGNAL_ENABLE
  if (sig_pending)
    {
      sig_pending = 0;

      ECB_MEMORY_FENCE;

      for (i = EV_NSIG - 1; i--; )
        if (expect_false (signals [i].pending))
          ev_feed_signal_event (loop, i + 1);
    }
#endif

#if EV_ASYNC_ENABLE
  if (async_pending)
    {
      async_pending = 0;

      ECB_MEMORY_FENCE;

      for (i = asynccnt; i--; )
        if (asyncs [i]->sent)
          {
            asyncs [i]->sent = 0;
            ECB_MEMORY_FENCE_RELEASE;
            ev_feed_event (loop, asyncs [i], EV_ASYNC);
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
  evpipe_write (loop, &sig_pending);
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
      ssize_t res = read (sigfd, si, sizeof (si));

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
static void
childcb (struct ev_loop *loop, ev_signal *sw, int revents)
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

unsigned int 
ev_recommended_backends (void)  
{
  unsigned int flags = ev_supported_backends ();

#ifndef __NetBSD__
  /* kqueue is borked on everything but netbsd apparently */
  /* it usually doesn't work correctly on anything but sockets and pipes */
  flags &= ~EVBACKEND_KQUEUE;
#endif
#ifdef __APPLE__
  /* only select works correctly on that "unix-certified" platform */
  flags &= ~EVBACKEND_KQUEUE; /* horribly broken, even for sockets */
  flags &= ~EVBACKEND_POLL;   /* poll is based on kqueue from 10.5 onwards */
#endif
#ifdef __FreeBSD__
  flags &= ~EVBACKEND_POLL;   /* poll return value is unusable (http://forums.freebsd.org/archive/index.php/t-10270.html) */
#endif

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

unsigned int
ev_backend (struct ev_loop *loop)  
{
  return backend;
}

#if EV_FEATURE_API
unsigned int
ev_iteration (struct ev_loop *loop)  
{
  return loop_count;
}

unsigned int
ev_depth (struct ev_loop *loop)  
{
  return loop_depth;
}

void
ev_set_io_collect_interval (struct ev_loop *loop, ev_tstamp interval)  
{
  io_blocktime = interval;
}

void
ev_set_timeout_collect_interval (struct ev_loop *loop, ev_tstamp interval)  
{
  timeout_blocktime = interval;
}

void
ev_set_userdata (struct ev_loop *loop, void *data)  
{
  userdata = data;
}

void *
ev_userdata (struct ev_loop *loop)  
{
  return userdata;
}

void
ev_set_invoke_pending_cb (struct ev_loop *loop, ev_loop_callback invoke_pending_cb)  
{
  invoke_cb = invoke_pending_cb;
}

void
ev_set_loop_release_cb (struct ev_loop *loop, void (*release)(struct ev_loop *loop)  , void (*acquire)(struct ev_loop *loop)  )  
{
  release_cb = release;
  acquire_cb = acquire;
}
#endif

/* initialise a loop structure, must be zero-initialised */
static void 
loop_init (struct ev_loop *loop, unsigned int flags)  
{
  if (!backend) {
      origflags = flags;

      /* pid check not overridable via env */
      if (flags & EVFLAG_FORKCHECK)
        loop->curpid = getpid ();

      if (!(flags & EVFLAG_NOENV) && !enable_secure() && getenv("LIBEV_FLAGS"))
        flags = atoi (getenv ("LIBEV_FLAGS"));

      ev_rt_now          = ev_time ();
      mn_now             = get_clock ();
      now_floor          = mn_now;
      rtmn_diff          = ev_rt_now - mn_now;
#if EV_FEATURE_API
      invoke_cb          = ev_invoke_pending;
#endif

      io_blocktime       = 0.;
      timeout_blocktime  = 0.;
      backend            = 0;
      backend_fd         = -1;
      sig_pending        = 0;
#if EV_ASYNC_ENABLE
      async_pending      = 0;
#endif
      pipe_write_skipped = 0;
      pipe_write_wanted  = 0;
      evpipe [0]         = -1;
      evpipe [1]         = -1;
#if EV_USE_INOTIFY
      fs_fd              = flags & EVFLAG_NOINOTIFY ? -1 : -2;
#endif
#if EV_USE_SIGNALFD
      sigfd              = flags & EVFLAG_SIGNALFD  ? -2 : -1;
#endif

      if (!(flags & EVBACKEND_MASK))
        flags |= ev_recommended_backends ();

#if EV_USE_EPOLL
      if (!backend && (flags & EVBACKEND_EPOLL )) backend = epoll_init  (loop, flags);
#endif
#if EV_USE_POLL
      if (!backend && (flags & EVBACKEND_POLL  )) backend = poll_init   (loop, flags);
#endif
#if EV_USE_SELECT
      if (!backend && (flags & EVBACKEND_SELECT)) backend = select_init (loop, flags);
#endif

      ev_prepare_init (&pending_w, pendingcb);

#if EV_SIGNAL_ENABLE || EV_ASYNC_ENABLE
      ev_init (&pipe_w, pipecb);
      ev_set_priority (&pipe_w, EV_MAXPRI);
#endif
    }
}

/* free up a loop structure */
void 
ev_loop_destroy (struct ev_loop *loop)
{
  int i;

  /* mimic free (0) */
  if (!loop)
    return;

#if EV_CLEANUP_ENABLE
  /* queue cleanup watchers (and execute them) */
  if (expect_false (cleanupcnt))
    {
      queue_events (loop, (W *)cleanups, cleanupcnt, EV_CLEANUP);
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

  if (ev_is_active (&pipe_w))
    {
      /*ev_ref (loop);*/
      /*ev_io_stop (loop, &pipe_w);*/

      if (evpipe [0] >= 0) close(evpipe [0]);
      if (evpipe [1] >= 0) close(evpipe [1]);
    }

#if EV_USE_SIGNALFD
  if (ev_is_active (&sigfd_w))
    close (sigfd);
#endif

#if EV_USE_INOTIFY
  if (fs_fd >= 0)
    close (fs_fd);
#endif

  if (backend_fd >= 0)
    close (backend_fd);

#if EV_USE_EPOLL
  if (backend == EVBACKEND_EPOLL ) epoll_destroy  (loop);
#endif
#if EV_USE_POLL
  if (backend == EVBACKEND_POLL  ) poll_destroy   (loop);
#endif
#if EV_USE_SELECT
  if (backend == EVBACKEND_SELECT) select_destroy (loop);
#endif

  for (i = NUMPRI; i--; )
    {
      array_free (pending, [i]);
#if EV_IDLE_ENABLE
      array_free (idle, [i]);
#endif
    }

  ev_free (anfds); anfds = 0; anfdmax = 0;

  /* have to use the microsoft-never-gets-it-right macro */
  array_free (rfeed, EMPTY);
  array_free (fdchange, EMPTY);
  array_free (timer, EMPTY);
#if EV_PERIODIC_ENABLE
  array_free (periodic, EMPTY);
#endif
#if EV_FORK_ENABLE
  array_free (fork, EMPTY);
#endif
#if EV_CLEANUP_ENABLE
  array_free (cleanup, EMPTY);
#endif
  array_free (prepare, EMPTY);
  array_free (check, EMPTY);
#if EV_ASYNC_ENABLE
  array_free (async, EMPTY);
#endif

  backend = 0;

  if (ev_is_default_loop (loop))
    ev_default_loop_ptr = 0;
  else
    ev_free (loop);
}

#if EV_USE_INOTIFY
static void infy_fork (struct ev_loop *loop);
#endif

static void
loop_fork (struct ev_loop *loop)
{
#if EV_USE_EPOLL
  if (backend == EVBACKEND_EPOLL ) epoll_fork  (loop);
#endif

#if EV_USE_INOTIFY
  infy_fork (loop);
#endif

#if EV_SIGNAL_ENABLE || EV_ASYNC_ENABLE
  if (ev_is_active (&pipe_w))
    {
      /* pipe_write_wanted must be false now, so modifying fd vars should be safe */

      ev_ref (loop);
      ev_io_stop (loop, &pipe_w);

      if (evpipe [0] >= 0)
        close(evpipe [0]);

      evpipe_init (loop);
      /* iterate over everything, in case we missed something before */
      ev_feed_event (loop, &pipe_w, EV_CUSTOM);
    }
#endif

  postfork = 0;
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
static void 
verify_watcher (struct ev_loop *loop, W w)
{
  assert (("libev: watcher has invalid priority", ABSPRI (w) >= 0 && ABSPRI (w) < NUMPRI));

  if (w->pending)
    assert (("libev: pending watcher not on pending queue", pendings [ABSPRI (w)][w->pending - 1].w == w));
}

static void 
verify_heap (struct ev_loop *loop, ANHE *heap, int N)
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

static void 
array_verify (struct ev_loop *loop, W *ws, int cnt)
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

  assert (activecnt >= -1);

  assert (fdchangemax >= fdchangecnt);
  for (i = 0; i < fdchangecnt; ++i)
    assert (("libev: negative fd in fdchanges", fdchanges [i] >= 0));

  assert (anfdmax >= 0);
  for (i = 0; i < anfdmax; ++i)
    {
      int j = 0;

      for (w = w2 = anfds [i].head; w; w = w->next)
        {
          verify_watcher (loop, (W)w);

          if (j++ & 1)
            {
              assert (("libev: io watcher list contains a loop", w != w2));
              w2 = w2->next;
            }

          assert (("libev: inactive fd watcher on anfd list", ev_active (w) == 1));
          assert (("libev: fd mismatch between watcher and anfd", ((ev_io *)w)->fd == i));
        }
    }

  assert (timermax >= timercnt);
  verify_heap (loop, timers, timercnt);

#if EV_PERIODIC_ENABLE
  assert (periodicmax >= periodiccnt);
  verify_heap (loop, periodics, periodiccnt);
#endif

  for (i = NUMPRI; i--; )
    {
      assert (pendingmax [i] >= pendingcnt [i]);
#if EV_IDLE_ENABLE
      assert (idleall >= 0);
      assert (idlemax [i] >= idlecnt [i]);
      array_verify (loop, (W *)idles [i], idlecnt [i]);
#endif
    }

#if EV_FORK_ENABLE
  assert (forkmax >= forkcnt);
  array_verify (loop, (W *)forks, forkcnt);
#endif

#if EV_CLEANUP_ENABLE
  assert (cleanupmax >= cleanupcnt);
  array_verify (loop, (W *)cleanups, cleanupcnt);
#endif

#if EV_ASYNC_ENABLE
  assert (asyncmax >= asynccnt);
  array_verify (loop, (W *)asyncs, asynccnt);
#endif

#if EV_PREPARE_ENABLE
  assert (preparemax >= preparecnt);
  array_verify (loop, (W *)prepares, preparecnt);
#endif

#if EV_CHECK_ENABLE
  assert (checkmax >= checkcnt);
  array_verify (loop, (W *)checks, checkcnt);
#endif

# if 0
#if EV_CHILD_ENABLE
  for (w = (ev_child *)childs [chain & ((EV_PID_HASHSIZE) - 1)]; w; w = (ev_child *)((WL)w)->next)
  for (signum = EV_NSIG; signum--; ) if (signals [signum].pending)
#endif
# endif
#endif
}
#endif

struct ev_loop * 
ev_default_loop (unsigned int flags)  
{
  if (!ev_default_loop_ptr)
    {
      struct ev_loop *loop = ev_default_loop_ptr = &default_loop_struct;

      loop_init (loop, flags);

      if (ev_backend (loop))
        {
#if EV_CHILD_ENABLE
          ev_signal_init (&childev, childcb, SIGCHLD);
          ev_set_priority (&childev, EV_MAXPRI);
          ev_signal_start (loop, &childev);
          ev_unref (loop); /* child watcher should not keep loop alive */
#endif
        }
      else
        ev_default_loop_ptr = 0;
    }

  return ev_default_loop_ptr;
}

void
ev_loop_fork (struct ev_loop *loop)  
{
  postfork = 1;
}

/*****************************************************************************/

void
ev_invoke (struct ev_loop *loop, void *w, int revents)
{
  EV_CB_INVOKE ((W)w, revents);
}

unsigned int
ev_pending_count (struct ev_loop *loop)  
{
  int pri;
  unsigned int count = 0;

  for (pri = NUMPRI; pri--; )
    count += pendingcnt [pri];

  return count;
}

void 
ev_invoke_pending (struct ev_loop *loop)
{
  pendingpri = NUMPRI;

  while (pendingpri) /* pendingpri possibly gets modified in the inner loop */
    {
      --pendingpri;

      while (pendingcnt [pendingpri])
        {
          ANPENDING *p = pendings [pendingpri] + --pendingcnt [pendingpri];

          p->w->pending = 0;
          EV_CB_INVOKE (p->w, p->events);
          EV_FREQUENT_CHECK;
        }
    }
}

#if EV_IDLE_ENABLE
/* make idle watchers pending. this handles the "call-idle */
/* only when higher priorities are idle" logic */
static void
idle_reify (struct ev_loop *loop)
{
  if (expect_false (idleall))
    {
      int pri;

      for (pri = NUMPRI; pri--; )
        {
          if (pendingcnt [pri])
            break;

          if (idlecnt [pri])
            {
              queue_events (loop, (W *)idles [pri], idlecnt [pri], EV_IDLE);
              break;
            }
        }
    }
}
#endif

/* make timers pending */
static void
timers_reify (struct ev_loop *loop)
{
  EV_FREQUENT_CHECK;

  if (timercnt && ANHE_at (timers [HEAP0]) < mn_now)
    {
      do
        {
          ev_timer *w = (ev_timer *)ANHE_w (timers [HEAP0]);

          /*assert (("libev: inactive timer on timer heap detected", ev_is_active (w)));*/

          /* first reschedule or stop timer */
          if (w->repeat)
            {
              ev_at (w) += w->repeat;
              if (ev_at (w) < mn_now)
                ev_at (w) = mn_now;

              assert (("libev: negative ev_timer repeat value found while processing timers", w->repeat > 0.));

              ANHE_at_cache (timers [HEAP0]);
              downheap (timers, timercnt, HEAP0);
            }
          else
            ev_timer_stop (loop, w); /* nonrepeating: stop timer */

          EV_FREQUENT_CHECK;
          feed_reverse (loop, (W)w);
        }
      while (timercnt && ANHE_at (timers [HEAP0]) < mn_now);

      feed_reverse_done (loop, EV_TIMER);
    }
}

#if EV_PERIODIC_ENABLE

static void 
periodic_recalc (struct ev_loop *loop, ev_periodic *w)
{
  ev_tstamp interval = w->interval > MIN_INTERVAL ? w->interval : MIN_INTERVAL;
  ev_tstamp at = w->offset + interval * ev_floor ((ev_rt_now - w->offset) / interval);

  /* the above almost always errs on the low side */
  while (at <= ev_rt_now)
    {
      ev_tstamp nat = at + w->interval;

      /* when resolution fails us, we use ev_rt_now */
      if (expect_false (nat == at))
        {
          at = ev_rt_now;
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

  while (periodiccnt && ANHE_at (periodics [HEAP0]) < ev_rt_now)
    {
      do
        {
          ev_periodic *w = (ev_periodic *)ANHE_w (periodics [HEAP0]);

          /*assert (("libev: inactive timer on periodic heap detected", ev_is_active (w)));*/

          /* first reschedule or stop timer */
          if (w->reschedule_cb)
            {
              ev_at (w) = w->reschedule_cb (w, ev_rt_now);

              assert (("libev: ev_periodic reschedule callback returned time in the past", ev_at (w) >= ev_rt_now));

              ANHE_at_cache (periodics [HEAP0]);
              downheap (periodics, periodiccnt, HEAP0);
            }
          else if (w->interval)
            {
              periodic_recalc (loop, w);
              ANHE_at_cache (periodics [HEAP0]);
              downheap (periodics, periodiccnt, HEAP0);
            }
          else
            ev_periodic_stop (loop, w); /* nonrepeating: stop timer */

          EV_FREQUENT_CHECK;
          feed_reverse (loop, (W)w);
        }
      while (periodiccnt && ANHE_at (periodics [HEAP0]) < ev_rt_now);

      feed_reverse_done (loop, EV_PERIODIC);
    }
}

/* simply recalculate all periodics */
/* TODO: maybe ensure that at least one event happens when jumping forward? */
static void 
periodics_reschedule (struct ev_loop *loop)
{
  int i;

  /* adjust periodics after time jump */
  for (i = HEAP0; i < periodiccnt + HEAP0; ++i)
    {
      ev_periodic *w = (ev_periodic *)ANHE_w (periodics [i]);

      if (w->reschedule_cb)
        ev_at (w) = w->reschedule_cb (w, ev_rt_now);
      else if (w->interval)
        periodic_recalc (loop, w);

      ANHE_at_cache (periodics [i]);
    }

  reheap (periodics, periodiccnt);
}
#endif

/* adjust all timers by a given offset */
static void 
timers_reschedule (struct ev_loop *loop, ev_tstamp adjust)
{
  int i;

  for (i = 0; i < timercnt; ++i)
    {
      ANHE *he = timers + i + HEAP0;
      ANHE_w (*he)->at += adjust;
      ANHE_at_cache (*he);
    }
}

/* fetch new monotonic and realtime times from the kernel */
/* also detect if there was a timejump, and act accordingly */
static void time_update (struct ev_loop *loop, ev_tstamp max_block)
{
	ev_rt_now = ev_time (); 
	if (expect_false (mn_now > ev_rt_now || ev_rt_now > mn_now + max_block + MIN_TIMEJUMP)) {
		/* adjust timers. this is easy, as the offset is the same for all of them */
		timers_reschedule (loop, ev_rt_now - mn_now);
#if EV_PERIODIC_ENABLE
        periodics_reschedule (loop);
#endif
	}

	mn_now = ev_rt_now;
}

int ev_run (struct ev_loop *loop, int flags)
{
#if EV_FEATURE_API
  ++loop_depth;
#endif

  assert (("libev: ev_loop recursion during release detected", loop_done != EVBREAK_RECURSE));

  loop_done = EVBREAK_CANCEL;

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
            postfork = 1;
          }

#if EV_FORK_ENABLE
      /* we might have forked, so queue fork handlers */
      if (expect_false (postfork))
        if (forkcnt)
          {
            queue_events (loop, (W *)forks, forkcnt, EV_FORK);
            EV_INVOKE_PENDING;
          }
#endif

#if EV_PREPARE_ENABLE
      /* queue prepare watchers (and execute them) */
      if (expect_false (preparecnt))
        {
          queue_events (loop, (W *)prepares, preparecnt, EV_PREPARE);
          EV_INVOKE_PENDING;
        }
#endif

      if (expect_false (loop_done))
        break;

      /* we might have forked, so reify kernel state if necessary */
      if (expect_false (postfork))
        loop_fork (loop);

      /* update fd-related kernel structures */
      fd_reify (loop);

      /* calculate blocking time */
      {
        ev_tstamp waittime  = 0.;
        ev_tstamp sleeptime = 0.;

        /* remember old timestamp for io_blocktime calculation */
        ev_tstamp prev_mn_now = mn_now;

        /* update time to cancel out callback processing overhead */
        time_update (loop, 1e100);

        /* from now on, we want a pipe-wake-up */
        pipe_write_wanted = 1;

        ECB_MEMORY_FENCE; /* make sure pipe_write_wanted is visible before we check for potential skips */

        if (expect_true (!(flags & EVRUN_NOWAIT || idleall || !activecnt || pipe_write_skipped)))
          {
            waittime = MAX_BLOCKTIME;

            if (timercnt)
              {
                ev_tstamp to = ANHE_at (timers [HEAP0]) - mn_now;
                if (waittime > to) waittime = to;
              }

#if EV_PERIODIC_ENABLE
            if (periodiccnt)
              {
                ev_tstamp to = ANHE_at (periodics [HEAP0]) - ev_rt_now;
                if (waittime > to) waittime = to;
              }
#endif

            /* don't let timeouts decrease the waittime below timeout_blocktime */
            if (expect_false (waittime < timeout_blocktime))
              waittime = timeout_blocktime;

            /* at this point, we NEED to wait, so we have to ensure */
            /* to pass a minimum nonzero value to the backend */
            if (expect_false (waittime < backend_mintime))
              waittime = backend_mintime;

            /* extra check because io_blocktime is commonly 0 */
            if (expect_false (io_blocktime))
              {
                sleeptime = io_blocktime - (mn_now - prev_mn_now);

                if (sleeptime > waittime - backend_mintime)
                  sleeptime = waittime - backend_mintime;

                if (expect_true (sleeptime > 0.))
                  {
                    ev_sleep (sleeptime);
                    waittime -= sleeptime;
                  }
              }
          }

#if EV_FEATURE_API
        ++loop_count;
#endif
        assert ((loop_done = EVBREAK_RECURSE, 1)); /* assert for side effect */
        backend_poll (loop, waittime);
        assert ((loop_done = EVBREAK_CANCEL, 1)); /* assert for side effect */

        pipe_write_wanted = 0; /* just an optimisation, no fence needed */

        ECB_MEMORY_FENCE_ACQUIRE;
        if (pipe_write_skipped)
          {
            assert (("libev: pipe_w not active, but pipe not written", ev_is_active (&pipe_w)));
            ev_feed_event (loop, &pipe_w, EV_CUSTOM);
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
      if (expect_false (checkcnt))
        queue_events (loop, (W *)checks, checkcnt, EV_CHECK);
#endif

      EV_INVOKE_PENDING;
    }
  while (expect_true (
    activecnt
    && !loop_done
    && !(flags & (EVRUN_ONCE | EVRUN_NOWAIT))
  ));

  if (loop_done == EVBREAK_ONE)
    loop_done = EVBREAK_CANCEL;

#if EV_FEATURE_API
  --loop_depth;
#endif

  return activecnt;
}

void
ev_break (struct ev_loop *loop, int how)  
{
  loop_done = how;
}

void
ev_ref (struct ev_loop *loop)  
{
  ++activecnt;
}

void
ev_unref (struct ev_loop *loop)  
{
  --activecnt;
}

void
ev_now_update (struct ev_loop *loop)  
{
  time_update (loop, 1e100);
}

void
ev_suspend (struct ev_loop *loop)  
{
  ev_now_update (loop);
}

void
ev_resume (struct ev_loop *loop)  
{
  ev_tstamp mn_prev = mn_now;

  ev_now_update (loop);
  timers_reschedule (loop, mn_now - mn_prev);
#if EV_PERIODIC_ENABLE
  /* TODO: really do this? */
  periodics_reschedule (loop);
#endif
}

/*****************************************************************************/
/* singly-linked list management, used when the expected list length is short */

static void
wlist_add (WL *head, WL elem)
{
  elem->next = *head;
  *head = elem;
}

static void
wlist_del (WL *head, WL elem)
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
static void
clear_pending (struct ev_loop *loop, W w)
{
  if (w->pending)
    {
      pendings [ABSPRI (w)][w->pending - 1].w = (W)&pending_w;
      w->pending = 0;
    }
}

int
ev_clear_pending (struct ev_loop *loop, void *w)  
{
  W w_ = (W)w;
  int pending = w_->pending;

  if (expect_true (pending))
    {
      ANPENDING *p = pendings [ABSPRI (w_)] + pending - 1;
      p->w = (W)&pending_w;
      w_->pending = 0;
      return p->events;
    }
  else
    return 0;
}

static void
pri_adjust (struct ev_loop *loop, W w)
{
  int pri = ev_priority (w);
  pri = pri < EV_MINPRI ? EV_MINPRI : pri;
  pri = pri > EV_MAXPRI ? EV_MAXPRI : pri;
  ev_set_priority (w, pri);
}

static void
ev_start (struct ev_loop *loop, W w, int active)
{
  pri_adjust (loop, w);
  w->active = active;
  ev_ref (loop);
}

static void
ev_stop (struct ev_loop *loop, W w)
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
  array_needsize (ANFD, anfds, anfdmax, fd + 1, array_init_zero);
  wlist_add (&anfds[fd].head, (WL)w);

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

  assert (("libev: ev_io_stop called with illegal fd (must stay constant after start!)", w->fd >= 0 && w->fd < anfdmax));

  EV_FREQUENT_CHECK;

  wlist_del (&anfds[w->fd].head, (WL)w);
  ev_stop (loop, (W)w);

  fd_change (loop, w->fd, EV_ANFD_REIFY);

  EV_FREQUENT_CHECK;
}

void ev_timer_start (struct ev_loop *loop, ev_timer *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  ev_at (w) += mn_now;

  assert (("libev: ev_timer_start called with negative timer repeat value", w->repeat >= 0.));

  EV_FREQUENT_CHECK;

  ++timercnt;
  ev_start (loop, (W)w, timercnt + HEAP0 - 1);
  array_needsize (ANHE, timers, timermax, ev_active (w) + 1, EMPTY2);
  ANHE_w (timers [ev_active (w)]) = (WT)w;
  ANHE_at_cache (timers [ev_active (w)]);
  upheap (timers, ev_active (w));

  EV_FREQUENT_CHECK;

  /*assert (("libev: internal timer heap corruption", timers [ev_active (w)] == (WT)w));*/
}

void ev_timer_stop (struct ev_loop *loop, ev_timer *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    assert (("libev: internal timer heap corruption", ANHE_w (timers [active]) == (WT)w));

    --timercnt;

    if (expect_true (active < timercnt + HEAP0))
      {
        timers [active] = timers [timercnt + HEAP0];
        adjustheap (timers, timercnt, active);
      }
  }

  ev_at (w) -= mn_now;

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
          ev_at (w) = mn_now + w->repeat;
          ANHE_at_cache (timers [ev_active (w)]);
          adjustheap (timers, timercnt, ev_active (w));
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
  return ev_at (w) - (ev_is_active (w) ? mn_now : 0.);
}

#if EV_PERIODIC_ENABLE
void ev_periodic_start (struct ev_loop *loop, ev_periodic *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  if (w->reschedule_cb)
    ev_at (w) = w->reschedule_cb (w, ev_rt_now);
  else if (w->interval)
    {
      assert (("libev: ev_periodic_start called with negative interval value", w->interval >= 0.));
      periodic_recalc (loop, w);
    }
  else
    ev_at (w) = w->offset;

  EV_FREQUENT_CHECK;

  ++periodiccnt;
  ev_start (loop, (W)w, periodiccnt + HEAP0 - 1);
  array_needsize (ANHE, periodics, periodicmax, ev_active (w) + 1, EMPTY2);
  ANHE_w (periodics [ev_active (w)]) = (WT)w;
  ANHE_at_cache (periodics [ev_active (w)]);
  upheap (periodics, ev_active (w));

  EV_FREQUENT_CHECK;

  /*assert (("libev: internal periodic heap corruption", ANHE_w (periodics [ev_active (w)]) == (WT)w));*/
}

void ev_periodic_stop (struct ev_loop *loop, ev_periodic *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    assert (("libev: internal periodic heap corruption", ANHE_w (periodics [active]) == (WT)w));

    --periodiccnt;

    if (expect_true (active < periodiccnt + HEAP0))
      {
        periodics [active] = periodics [periodiccnt + HEAP0];
        adjustheap (periodics, periodiccnt, active);
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
  if (sigfd == -2) {
      sigfd = signalfd (-1, &sigfd_set, SFD_NONBLOCK | SFD_CLOEXEC);
      if (sigfd < 0 && errno == EINVAL)
        sigfd = signalfd (-1, &sigfd_set, 0); /* retry without flags */

      if (sigfd >= 0)
        {
          fd_intern (sigfd); /* doing it twice will not hurt */

          sigemptyset (&sigfd_set);

          ev_io_init (&sigfd_w, sigfdcb, sigfd, EV_READ);
          ev_set_priority (&sigfd_w, EV_MAXPRI);
          ev_io_start (loop, &sigfd_w);
          ev_unref (loop); /* signalfd watcher should not keep loop alive */
        }
    }

  if (sigfd >= 0) {
      /* TODO: check .head */
      sigaddset (&sigfd_set, w->signum);
      sigprocmask (SIG_BLOCK, &sigfd_set, 0);

      signalfd (sigfd, &sigfd_set, 0);
    }
#endif

  ev_start (loop, (W)w, 1);
  wlist_add (&signals [w->signum - 1].head, (WL)w);

  if (!((WL)w)->next)
# if EV_USE_SIGNALFD
    if (sigfd < 0) /*TODO*/
# endif
      {
        struct sigaction sa;

        evpipe_init (loop);

        sa.sa_handler = ev_sighandler;
        sigfillset (&sa.sa_mask);
        sa.sa_flags = SA_RESTART; /* if restarting works we save one iteration */
        sigaction (w->signum, &sa, 0);

        if (origflags & EVFLAG_NOSIGMASK)
          {
            sigemptyset (&sa.sa_mask);
            sigaddset (&sa.sa_mask, w->signum);
            sigprocmask (SIG_UNBLOCK, &sa.sa_mask, 0);
          }
      }

  EV_FREQUENT_CHECK;
}

void 
ev_signal_stop (struct ev_loop *loop, ev_signal *w)  
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
      if (sigfd >= 0)
        {
          sigset_t ss;

          sigemptyset (&ss);
          sigaddset (&ss, w->signum);
          sigdelset (&sigfd_set, w->signum);

          signalfd (sigfd, &sigfd_set, 0);
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

void
ev_child_start (struct ev_loop *loop, ev_child *w)  
{
  assert (("libev: child watchers are only supported in the default loop", loop == ev_default_loop_ptr));
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, 1);
  wlist_add (&childs [w->pid & ((EV_PID_HASHSIZE) - 1)], (WL)w);

  EV_FREQUENT_CHECK;
}

void
ev_child_stop (struct ev_loop *loop, ev_child *w)  
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

static void 
infy_add (struct ev_loop *loop, ev_stat *w)
{
  w->wd = inotify_add_watch (fs_fd, w->path,
                             IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF | IN_MODIFY
                             | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO
                             | IN_DONT_FOLLOW | IN_MASK_ADD);

  if (w->wd >= 0)
    {
      struct statfs sfs;

      /* now local changes will be tracked by inotify, but remote changes won't */
      /* unless the filesystem is known to be local, we therefore still poll */
      /* also do poll on <2.6.25, but with normal frequency */

      if (!fs_2625)
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
              w->wd = inotify_add_watch (fs_fd, path, mask);
            }
          while (w->wd < 0 && (errno == ENOENT || errno == EACCES));
        }
    }

  if (w->wd >= 0)
    wlist_add (&fs_hash [w->wd & ((EV_INOTIFY_HASHSIZE) - 1)].head, (WL)w);

  /* now re-arm timer, if required */
  if (ev_is_active (&w->timer)) ev_ref (loop);
  ev_timer_again (loop, &w->timer);
  if (ev_is_active (&w->timer)) ev_unref (loop);
}

static void 
infy_del (struct ev_loop *loop, ev_stat *w)
{
  int slot;
  int wd = w->wd;

  if (wd < 0)
    return;

  w->wd = -2;
  slot = wd & ((EV_INOTIFY_HASHSIZE) - 1);
  wlist_del (&fs_hash [slot].head, (WL)w);

  /* remove this watcher, if others are watching it, they will rearm */
  inotify_rm_watch (fs_fd, wd);
}

static void 
infy_wd (struct ev_loop *loop, int slot, int wd, struct inotify_event *ev)
{
  if (slot < 0)
    /* overflow, need to check for all hash slots */
    for (slot = 0; slot < (EV_INOTIFY_HASHSIZE); ++slot)
      infy_wd (loop, slot, wd, ev);
  else
    {
      WL w_;

      for (w_ = fs_hash [slot & ((EV_INOTIFY_HASHSIZE) - 1)].head; w_; )
        {
          ev_stat *w = (ev_stat *)w_;
          w_ = w_->next; /* lets us remove this watcher and all before it */

          if (w->wd == wd || wd == -1)
            {
              if (ev->mask & (IN_IGNORED | IN_UNMOUNT | IN_DELETE_SELF))
                {
                  wlist_del (&fs_hash [slot & ((EV_INOTIFY_HASHSIZE) - 1)].head, (WL)w);
                  w->wd = -1;
                  infy_add (loop, w); /* re-add, no matter what */
                }

              stat_timer_cb (loop, &w->timer, 0);
            }
        }
    }
}

static void
infy_cb (struct ev_loop *loop, ev_io *w, int revents)
{
  char buf [EV_INOTIFY_BUFSIZE];
  int ofs;
  int len = read (fs_fd, buf, sizeof (buf));

  for (ofs = 0; ofs < len; )
    {
      struct inotify_event *ev = (struct inotify_event *)(buf + ofs);
      infy_wd (loop, ev->wd, ev->wd, ev);
      ofs += sizeof (struct inotify_event) + ev->len;
    }
}

static void 
ev_check_2625 (struct ev_loop *loop)
{
  /* kernels < 2.6.25 are borked
   * http://www.ussg.indiana.edu/hypermail/linux/kernel/0711.3/1208.html
   */
  if (ev_linux_version () < 0x020619)
    return;

  fs_2625 = 1;
}

static int
infy_newfd (void)
{
#if defined IN_CLOEXEC && defined IN_NONBLOCK
  int fd = inotify_init1 (IN_CLOEXEC | IN_NONBLOCK);
  if (fd >= 0)
    return fd;
#endif
  return inotify_init ();
}

static void
infy_init (struct ev_loop *loop)
{
  if (fs_fd != -2)
    return;

  fs_fd = -1;

  ev_check_2625 (loop);

  fs_fd = infy_newfd ();

  if (fs_fd >= 0)
    {
      fd_intern (fs_fd);
      ev_io_init (&fs_w, infy_cb, fs_fd, EV_READ);
      ev_set_priority (&fs_w, EV_MAXPRI);
      ev_io_start (loop, &fs_w);
      ev_unref (loop);
    }
}

static void
infy_fork (struct ev_loop *loop)
{
  int slot;

  if (fs_fd < 0)
    return;

  ev_ref (loop);
  ev_io_stop (loop, &fs_w);
  close (fs_fd);
  fs_fd = infy_newfd ();

  if (fs_fd >= 0)
    {
      fd_intern (fs_fd);
      ev_io_set (&fs_w, fs_fd, EV_READ);
      ev_io_start (loop, &fs_w);
      ev_unref (loop);
    }

  for (slot = 0; slot < (EV_INOTIFY_HASHSIZE); ++slot)
    {
      WL w_ = fs_hash [slot].head;
      fs_hash [slot].head = 0;

      while (w_)
        {
          ev_stat *w = (ev_stat *)w_;
          w_ = w_->next; /* lets us add this watcher */

          w->wd = -1;

          if (fs_fd >= 0)
            infy_add (loop, w); /* re-add, no matter what */
          else
            {
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

void
ev_stat_stat (struct ev_loop *loop, ev_stat *w)  
{
  if (lstat (w->path, &w->attr) < 0)
    w->attr.st_nlink = 0;
  else if (!w->attr.st_nlink)
    w->attr.st_nlink = 1;
}

static void 
stat_timer_cb (struct ev_loop *loop, ev_timer *w_, int revents)
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
        if (fs_fd >= 0)
          {
            infy_del (loop, w);
            infy_add (loop, w);
            ev_stat_stat (loop, w); /* avoid race... */
          }
      #endif

      ev_feed_event (loop, w, EV_STAT);
    }
}

void
ev_stat_start (struct ev_loop *loop, ev_stat *w)  
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

  if (fs_fd >= 0)
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

void
ev_stat_stop (struct ev_loop *loop, ev_stat *w)  
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
void
ev_idle_start (struct ev_loop *loop, ev_idle *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  pri_adjust (loop, (W)w);

  EV_FREQUENT_CHECK;

  {
    int active = ++idlecnt [ABSPRI (w)];

    ++idleall;
    ev_start (loop, (W)w, active);

    array_needsize (ev_idle *, idles [ABSPRI (w)], idlemax [ABSPRI (w)], active, EMPTY2);
    idles [ABSPRI (w)][active - 1] = w;
  }

  EV_FREQUENT_CHECK;
}

void
ev_idle_stop (struct ev_loop *loop, ev_idle *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    idles [ABSPRI (w)][active - 1] = idles [ABSPRI (w)][--idlecnt [ABSPRI (w)]];
    ev_active (idles [ABSPRI (w)][active - 1]) = active;

    ev_stop (loop, (W)w);
    --idleall;
  }

  EV_FREQUENT_CHECK;
}
#endif

#if EV_PREPARE_ENABLE
void
ev_prepare_start (struct ev_loop *loop, ev_prepare *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++preparecnt);
  array_needsize (ev_prepare *, prepares, preparemax, preparecnt, EMPTY2);
  prepares [preparecnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void
ev_prepare_stop (struct ev_loop *loop, ev_prepare *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    prepares [active - 1] = prepares [--preparecnt];
    ev_active (prepares [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_CHECK_ENABLE
void
ev_check_start (struct ev_loop *loop, ev_check *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++checkcnt);
  array_needsize (ev_check *, checks, checkmax, checkcnt, EMPTY2);
  checks [checkcnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void
ev_check_stop (struct ev_loop *loop, ev_check *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    checks [active - 1] = checks [--checkcnt];
    ev_active (checks [active - 1]) = active;
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

static void
embed_prepare_cb (struct ev_loop *loop, ev_prepare *prepare, int revents)
{
  ev_embed *w = (ev_embed *)(((char *)prepare) - offsetof (ev_embed, prepare));

  {
    struct ev_loop *loop = w->other;

    while (fdchangecnt)
      {
        fd_reify (loop);
        ev_run (loop, EVRUN_NOWAIT);
      }
  }
}

static void
embed_fork_cb (struct ev_loop *loop, ev_fork *fork_w, int revents)
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

#if 0
static void
embed_idle_cb (struct ev_loop *loop, ev_idle *idle, int revents)
{
  ev_idle_stop (loop, idle);
}
#endif

void
ev_embed_start (struct ev_loop *loop, ev_embed *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  {
    struct ev_loop *loop = w->other;
    assert (("libev: loop to be embedded is not embeddable", backend & ev_embeddable_backends ()));
    ev_io_init (&w->io, embed_io_cb, backend_fd, EV_READ);
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

void
ev_embed_stop (struct ev_loop *loop, ev_embed *w)  
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
void
ev_fork_start (struct ev_loop *loop, ev_fork *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++forkcnt);
  array_needsize (ev_fork *, forks, forkmax, forkcnt, EMPTY2);
  forks [forkcnt - 1] = w;

  EV_FREQUENT_CHECK;
}

void
ev_fork_stop (struct ev_loop *loop, ev_fork *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  {
    int active = ev_active (w);

    forks [active - 1] = forks [--forkcnt];
    ev_active (forks [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}
#endif

#if EV_CLEANUP_ENABLE
void
ev_cleanup_start (struct ev_loop *loop, ev_cleanup *w)  
{
  if (expect_false (ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;

  ev_start (loop, (W)w, ++cleanupcnt);
  array_needsize (ev_cleanup *, cleanups, cleanupmax, cleanupcnt, EMPTY2);
  cleanups [cleanupcnt - 1] = w;

  /* cleanup watchers should never keep a refcount on the loop */
  ev_unref (loop);
  EV_FREQUENT_CHECK;
}

void
ev_cleanup_stop (struct ev_loop *loop, ev_cleanup *w)  
{
  clear_pending (loop, (W)w);
  if (expect_false (!ev_is_active (w)))
    return;

  EV_FREQUENT_CHECK;
  ev_ref (loop);

  {
    int active = ev_active (w);

    cleanups [active - 1] = cleanups [--cleanupcnt];
    ev_active (cleanups [active - 1]) = active;
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

  ev_start (loop, (W)w, ++asynccnt);
  array_needsize (ev_async *, asyncs, asyncmax, asynccnt, EMPTY2);
  asyncs [asynccnt - 1] = w;

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

    asyncs [active - 1] = asyncs [--asynccnt];
    ev_active (asyncs [active - 1]) = active;
  }

  ev_stop (loop, (W)w);

  EV_FREQUENT_CHECK;
}

void ev_async_send (struct ev_loop *loop, ev_async *w)  
{
  w->sent = 1;
  evpipe_write (loop, &async_pending);
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

