# 用ucontext实现协程

GNU C库提供了一系列的用于创建，保存和切换用户态Context的函数，主要有如下4个：

+ `void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)`
	
+ `swapcontext(ucontext_t *oucp, ucontext_t *ucp)`
	
+ `int getcontext(ucontext_t *ucp)`
	
+ `int setcontext(const ucontext_t *ucp)`
	
