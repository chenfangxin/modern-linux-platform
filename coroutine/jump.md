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

----------------------------------------
## setjmp/longjmp机制

`setjmp/longjmp`机制方便程序从较深的调用栈快速返回，比如下面的例子：
```
func0()-->func1()-->func2()-->...-->funcN()
```
假设在`funcN`中发生了一个错误，需要快速返回`func0`中处理，按照通常的做法就是一级一级返回，这样效率很低。
现在如果在`func0`中，在调用`func1`之前，用`setjmp`保存一个状态，然后一旦调用路径中某个环节出现错误，
就可以用`longjmp`直接跳回`func0`。

