# 用setjmp/longjmp实现协程
 
## 原理
在标准C头文件`setjmp.h`中，定义了一组`setjmp/longjmp`函数，可以跨多层函数调用栈跳转，用于实现**非本地跳转**功能。

`setjmp/longjmp`函数具体接口如下：

+ `int setjmp(jmp_buf env)`

	该函数将当前函数的执行状态保存在`env`结构中，作为后续跳转执行的目标。
	
+ `void longjmp(jmp_buf env, int value)`

	该函数用来恢复`env`中保存的状态。参数`value`被用来设置`setjmp`的返回值。

由于`jmp_buf`结构导致实现栈切换功能有一定的困难，而且可移植性较差。

`libconcurrency`库采用了**栈拷贝**技术来实现协程的栈切换。

