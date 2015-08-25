#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "rte_timer.h"
#include "wheel_timer.h"
#include "min_heap_timer.h"
#include "rbtree_timer.h"
#include "skiplist_timer.h"

// #define RTE_WHEEL_TIMER
// #define RTE_MIN_HEAP_TIMER
// #define RTE_RBTREE_TIMER
#define RTE_SKIPLIST_TIMER

uint64_t rte_get_cur_time(void)
{
	struct timeval tv;	
	gettimeofday(&tv, NULL);

	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

int rte_timer_init(struct rte_timer *tim, void(*func)(struct rte_timer *), uint64_t data)
{
	memset(tim, 0, sizeof(struct rte_timer));
	INIT_LIST_HEAD(&(tim->list));
	tim->min_heap_idx = -1;
	tim->func = func;
	tim->data = data;
	tim->flags |= RTE_TIMER_INITED;

	return 0;
}

int rte_timer_system_init(void)
{
#if defined(RTE_WHEEL_TIMER)
	wheel_timer_system_init();
#elif defined(RTE_MIN_HEAP_TIMER)
	min_heap_timer_system_init();
#elif defined(RTE_RBTREE_TIMER)
	rbtree_timer_system_init();
#elif defined(RTE_SKIPLIST_TIMER)
	skiplist_timer_system_init();
#endif

	return 0;
}

int rte_add_timer(struct rte_timer *tim, uint32_t expire)
{
#if defined(RTE_WHEEL_TIMER)
	add_wheel_timer(tim, expire);
#elif defined(RTE_MIN_HEAP_TIMER)
	add_min_heap_timer(tim, expire);
#elif defined(RTE_RBTREE_TIMER)
	add_rbtree_timer(tim, expire);
#elif defined(RTE_SKIPLIST_TIMER)
	add_skiplist_timer(tim, expire);
#endif
	return 0;
}

int rte_del_timer(struct rte_timer *tim)
{
#if defined(RTE_WHEEL_TIMER)
	del_wheel_timer(tim);
#elif defined(RTE_MIN_HEAP_TIMER)
	del_min_heap_timer(tim);
#elif defined(RTE_RBTREE_TIMER)
	del_rbtree_timer(tim);
#elif defined(RTE_SKIPLIST_TIMER)
	del_skiplist_timer(tim);
#endif
	return 0;
}

int rte_modify_timer(struct rte_timer *tim, uint32_t expire)
{
#if defined(RTE_WHEEL_TIMER)
	modify_wheel_timer(tim, expire);
#elif defined(RTE_MIN_HEAP_TIMER)
	modify_min_heap_timer(tim, expire);
#elif defined(RTE_RBTREE_TIMER)
	modify_rbtree_timer(tim, expire);
#elif defined(RTE_SKIPLIST_TIMER)
	modify_skiplist_timer(tim, expire);
#endif
	return 0;
}

int rte_timer_manage(void)
{
#if defined(RTE_WHEEL_TIMER)
	wheel_timer_manage();
#elif defined(RTE_MIN_HEAP_TIMER)
	min_heap_timer_manage();
#elif defined(RTE_RBTREE_TIMER)
	rbtree_timer_manage();
#elif defined(RTE_SKIPLIST_TIMER)
	skiplist_timer_manage();
#endif
	return 0;
}
