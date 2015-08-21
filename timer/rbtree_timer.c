/*
 * 红黑树
 *
 * 代码参考Linux Kernel
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rte_thread.h"
#include "rbtree_timer.h"

static struct rbtree_timer_base global_rbt_base[RTE_THREAD_NUM];
int rbtree_timer_system_init(void)
{
	memset(global_rbt_base, 0, RTE_THREAD_NUM*sizeof(struct rbtree_timer_base));

	return 0;
}

int add_rbtree_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rbtree_timer_base *base=global_rbt_base + thread_idx;
	struct rb_node **link = &base->active.rb_node;
	struct rb_node *parent=NULL;
	struct rte_timer *entry=NULL;
	int leftmost=1;

	if(!(tim->flags & RTE_TIMER_INITED)){
		return -1;
	}

	if(tim->flags & RTE_TIMER_ADDED){
		return -1;
	}

	tim->expire = rte_get_cur_time() + expire;
	// tim->expire = expire;
	while(*link){
		parent = *link;
		entry = rb_entry(parent, struct rte_timer, node);
		if(tim->expire<entry->expire){
			link = &(*link)->rb_left;
		}else{
			link = &(*link)->rb_right;
			leftmost=0;
		}
	}
	if(leftmost){
		base->first = &tim->node;
	}
	rb_link_node(&tim->node, parent, link);
	rb_insert_color(&tim->node, &base->active);
	tim->flags |= RTE_TIMER_ADDED;

	return 0;
}

int del_rbtree_timer(struct rte_timer *tim)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rbtree_timer_base *base=global_rbt_base + thread_idx;

	if(base->first==&tim->node){
		base->first = rb_next(&tim->node);
	}
	rb_erase(&tim->node, &base->active);
	tim->flags &= ~RTE_TIMER_ADDED;

	return 0;
}

int modify_rbtree_timer(struct rte_timer *tim, uint32_t expire)
{
	// TODO
	return 0;
}

void rbtree_timer_manage(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rbtree_timer_base *base=global_rbt_base + thread_idx;
	uint64_t cur_usec = rte_get_cur_time();
	struct rb_node *node;
	struct rte_timer *tim;

	while((node=base->first)){
		tim = rb_entry(node, struct rte_timer, node);
		if(cur_usec<tim->expire){
			break;
		}
		del_rbtree_timer(tim);
		tim->func(tim);
	}
	return;
}
