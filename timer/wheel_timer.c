/*
 * 定时轮(Timing Wheel)
 *
 * 代码来自Linux内核
 */
#include <sys/time.h>
#include "wheel_timer.h"
#include "rte_thread.h"

static struct rte_tvec_base global_tvec_base[RTE_THREAD_NUM];
int wheel_timer_system_init(void)
{
	int i, j;
	struct rte_tvec_base *base=NULL;

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
		base->timer_usec = rte_get_cur_time();
	}

	return 0;
}

/*
 * */
static void internal_add_timer(struct rte_tvec_base *base, struct rte_timer *tim)
{
	uint64_t expire = tim->expire;
	uint64_t idx = expire-base->timer_usec;
	struct list_head *vec=NULL;
	int i;


	if(idx<TVR_SIZE){
		i = expire & TVR_MASK;
		vec = base->tv1.vec + i;
		// printf("In internal_add_timer, tv1, i=%d\n", i);
	}else if(idx<1<<(TVR_BITS + TVN_BITS)){
		i = (expire>>TVR_BITS) & TVN_MASK;
		vec = base->tv2.vec + i;
		// printf("In internal_add_timer, tv2, i=%d\n", i);
	}else if(idx<1<<(TVR_BITS + 2*TVN_BITS)){
		i = (expire>>(TVR_BITS+TVN_BITS)) & TVN_MASK;
		vec = base->tv3.vec + i;
		// printf("In internal_add_timer, tv3, i=%d\n", i);
	}else if(idx<1<<(TVR_BITS + 3*TVN_BITS)){
		i = (expire>>(TVR_BITS+2*TVN_BITS)) & TVN_MASK;
		vec = base->tv4.vec + i;
		// printf("In internal_add_timer, tv4, i=%d\n", i);
	}else if((int64_t)idx<0){
		vec = base->tv1.vec + (base->timer_usec & TVR_MASK);
	}else{
		if(idx>0xffffffffUL){
			idx = 0xffffffffUL;
			expire = idx + base->timer_usec;
		}
		i = (expire>>(TVR_BITS+3*TVN_BITS)) & TVN_MASK;
		vec = base->tv5.vec + i;
		// printf("In internal_add_timer, tv5, i=%d\n", i);
	}

	list_add_tail(&(tim->list), vec);

	return;
}

static void detach_timer(struct rte_timer *tim)
{
	struct list_head *list = &(tim->list);
	__list_del(list->prev, list->next);
	list->next = NULL;
	list->prev = RTE_LIST_POISION2;

	return;
}

static int cascade_timer(struct rte_tvec_base *base, struct rte_tvec *tv, int index)
{
	struct rte_timer *tim, *tmp;
	struct list_head tv_list;

	list_replace_init(tv->vec + index, &tv_list);
	list_for_each_entry_safe(tim, tmp, &tv_list, list){
		internal_add_timer(base, tim);
	}
	return index;
}

/*
 * 参数：
 *   expire 单位为 ms
 * */
int add_wheel_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rte_tvec_base *base=global_tvec_base+thread_idx;

	if(!(tim->flags & RTE_TIMER_INITED)){
		return 0;
	}

	if(tim->flags & RTE_TIMER_ADDED){
		return 0;
	}

	rte_mutex_lock(&(base->mutex));
	tim->expire = base->timer_usec + expire;
	internal_add_timer(base, tim);
	tim->flags |= RTE_TIMER_ADDED;
	rte_mutex_unlock(&(base->mutex));

	// printf("In add_wheel_timer, tim=%p, tim->expire=%ld\n", tim, tim->expire);

	return 0;
}

int del_wheel_timer(struct rte_timer *tim)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rte_tvec_base *base=global_tvec_base+thread_idx;

	rte_mutex_lock(&(base->mutex));
	if(!(tim->flags & RTE_TIMER_ADDED)){
		rte_mutex_unlock(&(base->mutex));
		return 0;
	}
	list_del(&tim->list);
	tim->flags &= ~(RTE_TIMER_ADDED);
	rte_mutex_unlock(&(base->mutex));

	return 0;
}

int modify_wheel_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rte_tvec_base *base=global_tvec_base+thread_idx;

	if(!(tim->flags & RTE_TIMER_INITED)){
		return 0;
	}
	rte_mutex_lock(&(base->mutex));
	if(!(tim->flags & RTE_TIMER_ADDED)){
		tim->expire = base->timer_usec + expire;
		internal_add_timer(base, tim);
		tim->flags |= RTE_TIMER_ADDED;
		rte_mutex_unlock(&(base->mutex));
		return 1;
	}
	list_del(&(tim->list));

	tim->expire = base->timer_usec + expire;
	internal_add_timer(base, tim);
	rte_mutex_unlock(&(base->mutex));

	return 1;
}

#define WHEEL_TIMER_INDEX(N) ((base->timer_usec>>(TVR_BITS+(N)*TVN_BITS))&TVN_MASK)
void wheel_timer_manage(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	struct rte_tvec_base *base = global_tvec_base+thread_idx;
	struct rte_timer *tim=NULL;
	uint64_t cur_usec=rte_get_cur_time();
	struct list_head work_list;
	struct list_head *head = &work_list;
	int index = base->timer_usec & TVR_MASK;

	if(cur_usec<=base->timer_usec){
		return;
	}

	rte_mutex_lock(&(base->mutex));
	if(!index && (!cascade_timer(base, &base->tv2, WHEEL_TIMER_INDEX(0))) &&
			(!cascade_timer(base, &base->tv3, WHEEL_TIMER_INDEX(1))) &&
			(!cascade_timer(base, &base->tv4, WHEEL_TIMER_INDEX(2)))){
		cascade_timer(base, &base->tv5, WHEEL_TIMER_INDEX(3));
	}

	list_replace_init(base->tv1.vec+index, &work_list);
	while(!list_empty(head)){
		tim = list_first_entry(head, struct rte_timer, list);
		detach_timer(tim);
		tim->flags &= ~(RTE_TIMER_ADDED);
		rte_mutex_unlock(&(base->mutex));
		tim->func(tim);
		rte_mutex_lock(&(base->mutex));
	}
	cur_usec = rte_get_cur_time();
	base->timer_usec = cur_usec;

	rte_mutex_unlock(&(base->mutex));
	return;
}
