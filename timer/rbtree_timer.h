#ifndef __RBTREE_TIMER_H__
#define __RBTREE_TIMER_H__
#include "rte_types.h"
#include "rte_timer.h"

struct rbtree_timer_base{
	struct rb_root active;
	struct rb_node *first;
};

int rbtree_timer_system_init(void);

int add_rbtree_timer(struct rte_timer *tim, uint32_t expire);
int del_rbtree_timer(struct rte_timer *tim);
int modify_rbtree_timer(struct rte_timer *tim, uint32_t expire);

void rbtree_timer_manage(void);
#endif
