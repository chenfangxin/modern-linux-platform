#ifndef ECB_H
#define ECB_H

#include <inttypes.h>

/*****************************************************************************/
/* http://www-01.ibm.com/support/knowledgecenter/SSGH3R_13.1.0/com.ibm.xlcpp131.aix.doc/compiler_ref/compiler_builtins.html */
#define ECB_MEMORY_FENCE         __asm__ __volatile__ ("mfence"   : : : "memory")
#define ECB_MEMORY_FENCE_ACQUIRE __asm__ __volatile__ (""         : : : "memory")
#define ECB_MEMORY_FENCE_RELEASE __asm__ __volatile__ ("")

/*****************************************************************************/
#define ecb_expect(expr,value)         __builtin_expect ((expr),(value))

/* put around conditional expressions if you are very sure that the  */
/* expression is mostly true or mostly false. note that these return */
/* booleans, not the expression.                                     */
#define ecb_expect_false(expr) ecb_expect (!!(expr), 0)
#define ecb_expect_true(expr)  ecb_expect (!!(expr), 1)
#endif
