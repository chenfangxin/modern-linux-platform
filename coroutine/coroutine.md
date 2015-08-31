# coroutine
  coroutine就是用户空间实现的轻量级线程，称为协程。其最大的好处就是协程之间切换完全在用户空间进行，代价非常低。

# 实现
  Linux系统上，有4中方法实现coroutine： setjmp/longjmp, switch, GCC label address, context。

# 示例
  jump使用setjmp/longjmp实现coroutine。
  protothreads库使用switch语句和GCC lable addrss扩展两种方式实现coroutine。
  libtask使用context相关函数实现coroutine。
  
