#include <stdio.h>
#include <stdlib.h>
#include <rte_timer.h>

void test_func(struct rte_timer *tim)
{
	printf("In test_func, tim->data=%ld\n",tim->data);

	rte_add_timer(tim, tim->data);
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
	struct rte_timer *tim=NULL;

	rte_timer_system_init();
#if 1
	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 40);
	rte_add_timer(tim, 40);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 15);
	rte_add_timer(tim, 15);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 30);
	rte_add_timer(tim, 30);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 17);
	rte_add_timer(tim, 17);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 2);
	rte_add_timer(tim, 2);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 50);
	rte_add_timer(tim, 50);

#endif
#if 0
	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 3000);
	rte_add_timer(tim, 3000);

	tim = (struct rte_timer *)malloc(sizeof(struct rte_timer));
	if(NULL==tim){
		return -1;
	}
	rte_timer_init(tim, test_func, 1000);
	rte_add_timer(tim, 1000);
	while(1)
#endif
	{

		rte_timer_manage();
	}
	return 0;
}
