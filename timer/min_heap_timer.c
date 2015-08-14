/* 
 * 堆排序
 * 堆是完全二叉树或近似完全二叉树(称为二叉堆)，其满足下面两个条件：
 * 1）父节点的键值总是小于或等于任意一个子节点的键值(称为最小堆或小根堆)
 * 2）每个节点的左子树和右子树都是一个堆
 *
 * 堆存储在数组中(因此对实际能添加的定时器个数有一定限制)，利用数组下标快速访问堆中元素。节点i(i>=0)的父节点下标为(i-1)/2; 左右子节点的下标分别为(2*i+1)和(2*i+2)
 *
 * 插入操作：
 *   1) 将元素放在堆数组的尾部
 *   2) 比较元素的键值如果小于其父节点，则将父节点元素与之交换，直到根节点
 *
 * 删除操作：
 *   删除发生在堆顶(称为POP)。其过程为先用最后一个元素取代堆顶，然后对堆进行调整，直到满足堆条件。
 *
 * 查找操作：
 *   最小堆的特性就是堆顶总是键值最小的元素。利用这一特性,不断的弹出堆顶元素，就会得到一个有序的序列。
 *
 * 代码来自Libevent库
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "min_heap_timer.h"
#include "rte_thread.h"

static min_heap_t global_min_heap[RTE_THREAD_NUM];

// #define RTE_HEAP_CAP (1UL<<20)
#define RTE_HEAP_CAP 16

int min_heap_timer_system_init(void)
{
	int i=0;
	min_heap_t *s=NULL;
	for(i=0;i<RTE_THREAD_NUM;i++){
		s = global_min_heap+i;
		s->p = (struct rte_timer  **)malloc(RTE_HEAP_CAP*sizeof(void *));
		if(NULL==s->p){
			printf("Failed to alloc heap.\n");
			return -1;
		}
		s->n = 0;
		s->a = RTE_HEAP_CAP;
	}

	return 0;
}

static int min_heap_elem_greater(struct rte_timer *a, struct rte_timer *p)
{
	return (a->expire>p->expire);
}

/*
 * 向上调整
 * */
static void min_heap_shift_up(min_heap_t *s, uint32_t hole_index, struct rte_timer *tim)
{
	uint32_t parent = hole_index?(hole_index-1)/2:hole_index;
	while(hole_index && min_heap_elem_greater(s->p[parent], tim)){
		(s->p[hole_index]=s->p[parent])->min_heap_idx = hole_index;
		hole_index = parent;
		parent = (hole_index-1)/2;
	}
	(s->p[hole_index] = tim)->min_heap_idx = hole_index;

	return;
}

/*
 * 向下调整
 * */
static void min_heap_shift_down(min_heap_t *s, uint32_t hole_index, struct rte_timer *tim)
{
	uint32_t min_child = 2 * (hole_index + 1); /* hole_index的右子节点 */
	while(min_child <= s->n){
		min_child -= (min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1])); /* 左子节点 */
		if(!min_heap_elem_greater(tim, s->p[min_child])){
			break;
		}
		(s->p[hole_index] = s->p[min_child])->min_heap_idx = hole_index;
		hole_index = min_child;
		min_child = 2 * (hole_index + 1);
	}
	(s->p[hole_index] = tim)->min_heap_idx = hole_index;

	return;
}

int add_min_heap_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	min_heap_t *s = global_min_heap + thread_idx;

	if(!(tim->flags & RTE_TIMER_INITED)){
		return 0;
	}

	if(tim->flags & RTE_TIMER_ADDED){
		return 0;
	}

	tim->expire = rte_get_cur_time() + expire;
	min_heap_shift_up(s, s->n++, tim);
	tim->flags |= RTE_TIMER_ADDED;

	return 0;
}

/*
 * 删除Heap中的某个元素，先用Heap中最后一个元素取代指定位置的元素，然后调整Heap
 * */
int del_min_heap_timer(struct rte_timer *tim)
{
	uint32_t thread_idx = rte_get_thread_id();
	min_heap_t *s = global_min_heap + thread_idx;
	struct rte_timer *last=NULL;

	if(-1!=tim->min_heap_idx){
		last = s->p[--s->n];
		s->p[s->n] = NULL;
		uint32_t parent = (tim->min_heap_idx-1)/2;
		if(tim->min_heap_idx>0 && min_heap_elem_greater(s->p[parent], last)){
			min_heap_shift_up(s, tim->min_heap_idx, last);
		}else{
			min_heap_shift_down(s, tim->min_heap_idx, last);
		}
		tim->min_heap_idx = -1;
		tim->flags &= ~RTE_TIMER_ADDED;
	}
	return 0;
}

static struct rte_timer *pop_min_heap_timer(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	min_heap_t *s = global_min_heap + thread_idx;
	struct rte_timer *tim=NULL;

	if(s->n){
		tim = s->p[0];
		min_heap_shift_down(s, 0ul, s->p[--s->n]);
		tim->min_heap_idx = -1;
		tim->flags &= ~RTE_TIMER_ADDED;
		return tim;
	}
	return NULL;
}

int modify_min_heap_timer(struct rte_timer *tim, uint32_t expire)
{
	//TODO
	return 0;
}

static struct rte_timer *min_heap_top(min_heap_t *s)
{
	return (s->n?*s->p:NULL);
}

static int min_heap_empty(min_heap_t *s)
{
	return (0u==s->n);
}

void min_heap_timer_manage(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	min_heap_t *s = global_min_heap + thread_idx;
	struct rte_timer *tim=NULL;
	uint64_t cur_usec = rte_get_cur_time();

	do{
		tim = min_heap_top(s);
		if(NULL==tim){
			break;
		}

		if(cur_usec<tim->expire){
			break;
		}

		pop_min_heap_timer();
		tim->func(tim);
	}while(min_heap_empty(s));

	return;
}
