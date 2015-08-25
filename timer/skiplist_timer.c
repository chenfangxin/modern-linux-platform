#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist_timer.h"
#include "rte_thread.h"

/* 用于timer 排序的函数 */
static inline int cmp_node(void *orig, void *new)
{
	return ((struct rte_timer *)orig)->expire < ((struct rte_timer *)new)->expire;
}

static skiplist *global_skiplist[RTE_THREAD_NUM];
int skiplist_timer_system_init(void)
{
	int i=0;
	skiplist *sl=NULL;

	for(i=0;i<RTE_THREAD_NUM;i++){
		sl = slCreate(&cmp_node);
		if(NULL==sl){
			return -1;
		}
		global_skiplist[i] = sl;
	}

	return 0;
}

int add_skiplist_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];

	if(!(tim->flags & RTE_TIMER_INITED)){
		return -1;
	}

	if(tim->flags & RTE_TIMER_ADDED){
		return -1;
	}

	tim->expire = rte_get_cur_time() + expire;
	slInsert(sl, tim);
	tim->flags |= RTE_TIMER_ADDED;

	return 0;
}

int del_skiplist_timer(struct rte_timer *tim)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];

	if(!(tim->flags & RTE_TIMER_ADDED)){
		return -1;
	}
	slDelete(sl, tim);
	tim->flags &= ~RTE_TIMER_ADDED;

	return 0;
}

int modify_skiplist_timer(struct rte_timer *tim, uint32_t expire)
{
	// TODO
	return 0;
}

void skiplist_timer_manage(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];
	skiplistNode *node=NULL;
	struct rte_timer *tim=NULL;
	uint64_t cur_usec = rte_get_cur_time();

	while((node=sl->header->forward[0])!=NULL){
		tim = node->obj;
		if(cur_usec<tim->expire){
			break;
		}
		slDelete(sl, tim);
		tim->flags &= ~RTE_TIMER_ADDED;

		tim->func(tim);
	}
	return;
}


