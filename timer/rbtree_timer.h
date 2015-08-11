#ifndef __RBTREE_TIMER_H__
#define __RBTREE_TIMER_H__
#include "rte_timer.h"

int add_rbtree_timer(struct rte_timer *tim, uint32_t expire);
int del_rbtree_timer(struct rte_timer *tim);
int modify_rbtree_timer(struct rte_timer *tim, uint32_t expire);

void rbtree_timer_manage(void);
#endif
