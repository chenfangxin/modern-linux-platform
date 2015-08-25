#ifndef __RTE_HASH_H__
#define __RTE_HASH_H__
#include "rte_types.h"

/* From Folly library */
static inline uint64_t hash_128_to_64(const uint64_t upper, const uint64_t lower)
{
  const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  uint64_t a = (lower ^ upper) * kMul;
  a ^= (a >> 47);
  uint64_t b = (upper ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

#endif
