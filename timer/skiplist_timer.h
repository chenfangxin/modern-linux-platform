#ifndef __SKIPLIST_TIMER_H__
#define __SKIPLIST_TIMER_H__

#include "rte_timer.h"

#define SKIPLIST_MAXLEVEL 8 /* 最大层级 */
#define SKIPLIST_P 0.5 /* 下级元素出现在上一级的概率 */

typedef struct skiplistNode{
	struct rte_timer *tim;
	struct skiplistNode *forward[0]; // Level
}skiplistNode;

typedef struct skiplist{
	struct skiplistNode *header;
	unsigned long length;
	int level;
}skiplist;

int skiplist_timer_system_init(void);

int add_skiplist_timer(struct rte_timer *tim, uint32_t expire);
int del_skiplist_timer(struct rte_timer *tim);
int modify_skiplist_timer(struct rte_timer *tim, uint32_t expire);

void skiplist_timer_manage(void);

#endif
