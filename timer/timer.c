#include <stdio.h>
#include <stdlib.h>
#include <rte_timer.h>
#include "wheel_timer.h"
#include "min_heap_timer.h"
#include "rbtree_timer.h"

// #define RTE_WHEEL_TIMER
#define RTE_MIN_HEAP_TIMER
// #define RTE_RBTREE_TIMER

void test_func1(struct rte_timer *tim)
{
	printf("In test_func1, usec=%ld\n", rte_get_cur_time());
#ifdef RTE_WHEEL_TIMER
	add_wheel_timer(tim, 5000);
#endif
#ifdef RTE_MIN_HEAP_TIMER
	add_min_heap_timer(tim, 5000);
#endif
#ifdef RTE_RBTREE_TIMER
	add_rbtree_timer(tim, 5000);
#endif
}

void test_func2(struct rte_timer *tim)
{
	printf("In test_func2, usec=%ld\n", rte_get_cur_time());
#ifdef RTE_WHEEL_TIMER
	add_wheel_timer(tim, 1000);
#endif
#ifdef RTE_MIN_HEAP_TIMER
	add_min_heap_timer(tim, 1000);
#endif
#ifdef RTE_RBTREE_TIMER
	add_rbtree_timer(tim, 1000);
#endif
}

// int pid;
int main(void)
{
#if 0
	__asm__(
			"movl $20, %eax	\n"
			"call *%gs:0x10	\n"
			"movl %eax, pid	\n"
	);
	printf("pid is %d\n", pid);
#endif
#if 0
	int i;
	struct timeval tv;
	for(i=0;i<1000000;i++){
		if(gettimeofday(&tv, NULL)){
			perror("gtod");
		}
	}
#endif
	struct rte_timer *tim1=NULL, *tim2=NULL;
	tim1 = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	tim2 = (struct rte_timer *)malloc(sizeof(struct rte_timer));

	rte_timer_init(tim1, test_func1, 0);
	rte_timer_init(tim2, test_func2, 0);

#ifdef RTE_WHEEL_TIMER
	wheel_timer_system_init();

	add_wheel_timer(tim1, 5000);
	add_wheel_timer(tim2, 1000);

	while(1){
		wheel_timer_manage();
	}
#endif

#ifdef RTE_MIN_HEAP_TIMER
	min_heap_timer_system_init();

	add_min_heap_timer(tim1, 5000);
	add_min_heap_timer(tim2, 1000);

	while(1){
		min_heap_timer_manage();
	}
#endif

#ifdef RTE_RBTREE_TIMER
	rbtree_timer_system_init();

	add_rbtree_timer(tim1, 5000);
	add_rbtree_timer(tim2, 1000);

	while(1){
		rbtree_timer_manage();
	}
#endif

	return 0;
}
