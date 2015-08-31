#include <stdio.h> 

struct pt{
	unsigned short lc;
};

static int protothread1_flag, protothread2_flag;

static int protothread1(struct pt *pt)
{
	// PT_BEGIN
	switch(pt->lc){ // 利用switch语句实现函数体内跳转
		case 0:
			while(1) {
				pt->lc=__LINE__;
		case __LINE__:
			if(!(protothread2_flag!=0)) {
				return 0;
			}
    		printf("Protothread 1 running\n");
    		protothread2_flag = 0;
			protothread1_flag = 1;
  		}
	}; 

	// PT_END
	pt->lc=0;
	return 3; 
}

static int protothread2(struct pt *pt)
{
	// PT_BEGIN
	switch((pt)->lc){ // 利用switch语句实现函数体内跳转
		case 0:
			 while(1){
				protothread2_flag = 1;
				pt->lc=__LINE__;
		case __LINE__:
			if(!(protothread1_flag!=0)) {
				return 0;
			}
			printf("Protothread 2 running\n");
			protothread1_flag = 0;
		}
	}; 

	// PT_END
	(pt)->lc=0; 
	return 3; 
}

static struct pt pt1, pt2;

int main(void)
{
	pt1.lc = 0;
	pt2.lc = 0;

	while(1) {
		protothread1(&pt1);
		protothread2(&pt2);
	}
}
