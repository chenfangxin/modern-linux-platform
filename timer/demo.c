#include <stdio.h>
#include <sys/time.h>

int pid;
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
	int i;
	struct timeval tv;
	for(i=0;i<1000000;i++){
		if(gettimeofday(&tv, NULL)){
			perror("gtod");
		}
	}

	return 0;
}
