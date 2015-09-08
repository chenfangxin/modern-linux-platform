#ifndef ECB_H
#define ECB_H

#include <inttypes.h>

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
      #define ECB_MEMORY_FENCE         __asm__ __volatile__ ("mfence"   : : : "memory")
      #define ECB_MEMORY_FENCE_ACQUIRE __asm__ __volatile__ (""         : : : "memory")
      #define ECB_MEMORY_FENCE_RELEASE __asm__ __volatile__ ("")
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
