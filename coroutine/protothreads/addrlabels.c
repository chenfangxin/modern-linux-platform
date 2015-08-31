#include <stdio.h> 

#define LC_CONCAT2(s1, s2) s1##s2
#define LC_CONCAT(s1, s2) LC_CONCAT2(s1, s2)

#define LC_SET(s)							\
  do {										\
    LC_CONCAT(LC_LABEL, __LINE__):   	    \
    (s) = &&LC_CONCAT(LC_LABEL, __LINE__);	\
  } while(0)

struct pt{
	void *lc;
};

static int protothread1_flag, protothread2_flag;

static int protothread1(struct pt *pt)
{
	// PT_BEGIN
	if(pt->lc != NULL) {
		goto *(pt->lc); // 跳转到存储的地址
	}

	while(1) {
		LC_SET((pt)->lc); // 利用GCC的扩展特性，存储labe的地址
		if(!(protothread2_flag!=0)) {
			return 0;
		}
    	printf("Protothread 1 running\n");

   		protothread2_flag = 0;
	    protothread1_flag = 1;
	}

	// PT_END
	pt->lc=NULL;
	return 3;
}

static int protothread2(struct pt *pt)
{
	// PT_BEGIN
	if(pt->lc != NULL) {
		goto *(pt->lc); // 跳转到存储的地址
	}

	while(1) {
		protothread2_flag = 1;
		LC_SET((pt)->lc); // 利用GCC特性存储label的地址
		if(!(protothread1_flag!=0)) {
			return 0;
		}
    	printf("Protothread 2 running\n");
    	protothread1_flag = 0;
  	}

	// PT_END
	pt->lc = NULL;
	return 3;
}

static struct pt pt1, pt2;

int main(void)
{
	pt1.lc = NULL;
	pt2.lc = NULL;

	while(1){
		protothread1(&pt1);
		protothread2(&pt2);
	}
}
