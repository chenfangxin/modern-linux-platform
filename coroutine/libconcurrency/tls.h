/*
 * Compiler-provided Thread-Local Storage
 * TLS declarations for various compilers:
 * http://en.wikipedia.org/wiki/Thread-local_storage
 */

#ifndef __TLS_H__
#define __TLS_H__

/* assume gcc or compatible compiler */
#define THREAD_LOCAL __thread

#endif /*__TLS_H__*/
