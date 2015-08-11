#include <stdio.h>
#include <stdlib.h>
#include <rte_timer.h>
#include "wheel_timer.h"

void test_func(struct rte_timer *tim)
{		
	printf("In test_func, usec=%ld\n", rte_get_cur_time());

	add_wheel_timer(tim, 5000);
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
	struct rte_timer *tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	rte_timer_init(tim, test_func, 0);

	wheel_timer_system_init();	

	add_wheel_timer(tim, 5000);

	while(1){
		wheel_timer_manage();
	}

	return 0;
}
