# 用ucontext实现协程

GNU C库提供了一系列的用于创建，保存和切换用户态Context的函数，主要有如下4个：

+ `void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)`
	
	该函数用于初始化一个`ucontext_t`结构体。参数`func`指向该`context`的入口函数；`argc`指明参数个数。

+ `int setcontext(const ucontext_t *ucp)`
	
	该函数用来将执行线索切换到`ucp`指明的`context`。

+ `int getcontext(ucontext_t *ucp)`
	
	该函数用来将当前执行状态保存到`ucontext_t`结构体中。若后续调用`setcontext`或`swapcontext`函数，
	恢复运行状态，程序会沿着`getcontext`调用点之后继续执行，就像从`getcontext`函数返回一样。

+ `swapcontext(ucontext_t *oucp, ucontext_t *ucp)`
	
	由于`getcontext`函数，不能区分返回状态，在编写`context`切换代码时，需要额外的信息来判断。为了简化
	切换操作，提供了`swapcontext`函数。但是该函数使用了内核锁，切换效率不高。

实际上，只用`setcontext/getcontext`就能实现`coroutine`。
	
