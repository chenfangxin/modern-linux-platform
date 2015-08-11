#定时器的实现
##时钟源
    用`gettimeofday`来获得当前时间，原因如下：

1. 在`x86_64`平台上，`gettimeofday`是用户空间实现的(见vsyscall)，没有系统调用的上下文切换和陷入内核的开销。
2. `gettimeofday`的精度是1ms

	用timerfd实现超时处理，可以用在select/poll/epoll中统一处理。
	
