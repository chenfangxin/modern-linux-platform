#定时器的实现
##时钟源
    用`gettimeofday`来获得当前时间，原因如下：

1. 在`x86_64`平台上，`gettimeofday`是用户空间实现的(见vsyscall)，没有系统调用的上下文切换和陷入内核的开销。
2. `gettimeofday`的精度是1ms

	用timerfd实现超时处理，可以用在select/poll/epoll中统一处理。

## wheel timer
    基于timing wheel模型实现的定时器, Linux内核中的timer由此模型实现。

## min heap timer
    用最小堆实现的定时器，Libevent库中的timer有此模型实现。

## rbtree timer
	用红黑树实现的定时器，Linux内核中的hrtimer，Nginx中的timer由此模型实现。

## skiplist timer
    用skiplist实现的定时器
