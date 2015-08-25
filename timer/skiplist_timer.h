#ifndef __SKIPLIST_TIMER_H__
#define __SKIPLIST_TIMER_H__
#include "skiplist.h"
#include "rte_timer.h"


int skiplist_timer_system_init(void);

int add_skiplist_timer(struct rte_timer *tim, uint32_t expire);
int del_skiplist_timer(struct rte_timer *tim);
int modify_skiplist_timer(struct rte_timer *tim, uint32_t expire);

void skiplist_timer_manage(void);

#endif
