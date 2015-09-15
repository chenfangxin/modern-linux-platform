# coroutine
  coroutine就是用户空间实现的轻量级线程，称为协程。其最大的好处就是协程之间切换完全在用户空间进行，代价非常低。

  coroutine有两个典型应用，一个是实现generator；另一个是事件处理模型中，化异步为同步。

# 实现
  Linux系统上，有3中方法实现coroutine： setjmp/longjmp, switch+__LINE__/GCC label address, context。

# 示例
  protothreads库使用switch语句和GCC lable addrss扩展两种方式实现coroutine。

## setjmp/longjmp
  jump使用setjmp/longjmp实现coroutine。
  使用setjmp/longjmp有限制，就是必须先调用setjmp，然后才能调用longjmp，而且jmp_buf中存放的内容没有明确的定义。

## context function
  libtask使用context相关函数实现coroutine。
  

