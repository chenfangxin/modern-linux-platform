#ifndef __MIN_HEAP_TIMER_H__
#define __MIN_HEAP_TIMER_H__

#include "rte_timer.h"

typedef struct min_heap{
	struct rte_timer **p; /* 存放元素的数组 */
	uint32_t n; /* 堆中当前存放的元素个数 */
	uint32_t a; /* 数组的容量 */
}min_heap_t;

int min_heap_timer_system_init(void);
int add_min_heap_timer(struct rte_timer *tim, uint32_t expire);
int del_min_heap_timer(struct rte_timer *tim);
int modify_min_heap_timer(struct rte_timer *tim, uint32_t expire);

void min_heap_timer_manage(void);

#endif
