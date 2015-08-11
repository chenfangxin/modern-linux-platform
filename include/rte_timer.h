#ifndef __RTE_TIMER_H__
#define __RTE_TIMER_H__
#include "rte_types.h"
#include "rte_list.h"

#define RTE_TIMER_INITED	(1UL<<0)
#define RTE_TIMER_ADDED		(1UL<<1)

struct rte_timer;

struct rte_timer{
	struct list_head list;
	uint32_t flags;
	uint64_t expire;
	void (*func)(struct rte_timer *);
	uint64_t data;
};

uint64_t rte_get_cur_time(void);

#define rte_time_after(a,b) \
		(typecheck(unsigned long, a) && \
		 typecheck(unsigned long, b) && \
		 ((long)(b)-(long)(a)<0))

#define rte_time_after_eq(a,b) \
		(typecheck(unsigned long, a) && \
		 typecheck(unsigned long, b) && \
		 ((long)(a)-(long)(b)>=0))

int rte_timer_init(struct rte_timer *tim, void(*func)(struct rte_timer *), uint64_t data);
#endif
