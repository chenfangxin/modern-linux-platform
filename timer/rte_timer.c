#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "rte_timer.h"

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
	tim->func = func;
	tim->data = data;
	tim->flags |= RTE_TIMER_INITED;

	return 0;
}
