/* 
 * 堆排序
 *
 * 代码来自Libevent库
 */
#include <stdio.h>
#include <stdlib.h>
#include "min_heap_timer.h"

static void min_heap_ctor(min_heap_t *s)
{
	s->p = NULL;
	s->n = 0;
	s->a = 0;

	return;
}

static void min_heap_dtor(min_heap_t *s)
{
	if(NULL!=s->p){
		free(s->p);
		min_heap_ctor(s);
	}
	return;
}

int add_min_heap_timer(struct rte_timer *tim, uint32_t expire)
{
	return 0;
}

int del_min_heap_timer(struct rte_timer *tim)
{
	return 0;
}

int modify_min_heap_timer(struct rte_timer *tim, uint32_t expire)
{
	return 0;
}

void min_heap_timer_manage(void)
{
	return;
}
