#ifndef __WHEEL_TIMER_H__
#define __WHEEL_TIMER_H__
#include <stdio.h>
#include "rte_list.h"
#include "rte_mutex.h"
#include "rte_timer.h"

#define TVR_BITS 8
#define TVN_BITS 6
#define TVR_SIZE (1<<TVR_BITS)
#define TVN_SIZE (1<<TVN_BITS)
#define TVN_MASK (TVN_SIZE-1)
#define TVR_MASK (TVR_SIZE-1)

struct rte_tvec{
	struct list_head vec[TVN_SIZE];
};

struct rte_tvec_root{
	struct list_head vec[TVR_SIZE];
};

 /*
  * wheel_timer模型实际上是以链表来组织timer
  * */
struct rte_tvec_base{
	rte_mutex_t mutex;
	uint64_t timer_usec;
	struct list_head work_list;
	struct rte_tvec_root tv1; /* tv1有256个时间单位，每个1ms，总共256ms */
	struct rte_tvec tv2; /* tv2有64个时间单位，每个256ms，总共16384ms */
	struct rte_tvec tv3; /* tv3有64个时间单位，每个16384ms，总共17.47min */
	struct rte_tvec tv4; /* tv4有64个时间单位，每个17.47min，总共18.64hour */
	struct rte_tvec tv5; /* tv5有64个时间单位，每个18.64hour，总共49.71day */
};

int wheel_timer_system_init(void);
int add_wheel_timer(struct rte_timer *tim, uint32_t expire);
int del_wheel_timer(struct rte_timer *tim);
int modify_wheel_timer(struct rte_timer *tim, uint32_t expire);

void wheel_timer_manage(void);

#endif
