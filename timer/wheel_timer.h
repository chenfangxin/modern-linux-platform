#ifndef __WHEEL_TIMER_H__
#define __WHEEL_TIMER_H__
#include "rte_list.h"
#include "rte_mutex.h"

#define TVN_BITS 6
#define TVR_BITS 8
#define TVN_SIZE (1<<TVN_BITS)
#define TVR_BITS (1<<TVR_BITS)
#define TVN_MASK (TVN_SIZE-1)
#define TVR_MASK (TVR_SIZE-1)

struct rte_tvec{
	struct list_head vec[TVN_SIZE];
};

struct rte_tvec_root{
	uint32_t index;
	struct list_head vec[TVR_SIZE];
};

struct rte_tvec_base{
	rte_mutex_t mutex;
	unsigned long timer_usec;
	struct list_head work_list;
	struct tvec_root tv1;
	struct tvec tv2;
	struct tvec tv3;
	struct tvec tv4;
	struct tvec tv5;
};

struct rte_timer{
	struct list_head list;
	uint32_t expire;
	void (*func)(uint64_t);
	uint64_t data;
	struct rte_tvec_base *base;
};

int rte_add_timer(struct rte_timer *tim, uint32_t expire);
int rte_stop_timer(struct rte_timer *tim, uint32_t expire);
int rte_modify_timer(struct rte_timer *tim, uint32_t expire);
void rte_timer_manage(void);
#endif
