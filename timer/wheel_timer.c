/*
 * 定时轮(Timing Wheel)
 *
 * 代码来自Linux内核
 */
#include <sys/time.h>
#include "wheel_timer.h"
#include "rte_thread.h"

#define RTE_THREAD_NUM 4
static struct rte_tvec_base global_tvec_base[RTE_THREAD_NUM];

int wheel_timer_system_init(void)
{
	int i, j;
	struct rte_tvec_base *base=NULL;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	for(i=0;i<RTE_THREAD_NUM;i++){
		base = global_tvec_base+i;
		rte_mutex_init(&base->mutex);
		INIT_LIST_HEAD(&base->work_list);
		for(j=0;j<TVN_SIZE;j++){
			INIT_LIST_HEAD(base->tv2.vec+j);
			INIT_LIST_HEAD(base->tv3.vec+j);
			INIT_LIST_HEAD(base->tv4.vec+j);
			INIT_LIST_HEAD(base->tv5.vec+j);
		}
		for(j=0;j<TVR_SIZE;j++){
			INIT_LIST_HEAD(base->tv1.vec+j);
		}
		base->timer_usec = tv.tv_usec;
	}

	return 0;
}

int rte_init_timer(struct rte_timer *tim, void (*func)(uint64_t), uint64_t data)
{

	return 0;
}

static void internal_add_timer(struct rte_tvec_base *base, struct rte_timer *tim)
{

	return;
}

int rte_add_timer(struct rte_timer *tim, uint32_t expire)
{
	return 0;
}

int rte_stop_timer(struct rte_timer *tim)
{
	return 0;
}

int rte_modify_timer(struct rte_timer *tim, uint32_t expire)
{
	return 0;
}

static void detach_timer(struct rte_timer *tim)
{
	return;
}

static int cascade_timer(struct rte_tvec_base *base, struct rte_tvec *tv)
{
	return 0;
}

void wheel_timer_manage(void)
{
	uint32_t thread_idx = rte_get_self_id();
	struct rte_tvec_base *base = global_tvec_base+thread_idx;

	rte_mutex_lock(&base->mutex);
	do{

	}while();
	rte_mutex_unlock(&base->mutex);
	return;
}
