# reactor
## 简介
  reactor是`non-blocking IO + IO multiplexing`模型。在这种模型中，程序结构基本上就是一个事件循环（event loop），以事件驱动和事件回调的方式实现业务逻辑。  

## libev
  libev是一个功能强大的reactor实现，在Linux平台上支持select/poll/epoll。

本示例简化代码，只支持x86_64平台，Linux系统，GCC>=4.9
