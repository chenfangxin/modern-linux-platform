#ifndef __RTE_LIKELY_H__
#define __RTE_LIKELY_H__

// __GNU_C__ >= 4

#define likely(x)   (__builtin_expect((x), 1))
#define unlikely(x) (__builtin_expect((x), 0))

#endif
