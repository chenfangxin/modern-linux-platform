CC=gcc
CFLAGS = -g
# CFLAGS += -fPIC -Wall -Wunused-function -Wextra
CFLAGS += -fPIC -Wunused-function -Wextra
CFLAGS += -DHAVE_PTHREAD_PRIO_INHERIT -DHAVE_EVENTFD -DNN_HAVE_EVENTFD -DHAVE_PIPE -DNN_HAVE_PIPE -DHAVE_PIPE2 -DNN_HAVE_PIPE2 -DNN_HAVE_CLOCK_MONOTONIC -DHAVE_CLOCK_GETTIME -DHAVE_POLL -DNN_HAVE_POLL -DHAVE_EPOLL_CREATE -DNN_USE_EPOLL -DHAVE_GETIFADDRS -DNN_USE_IFADDRS -DHAVE_ACCEPT4 -DNN_HAVE_ACCEPT4 -DNN_HAVE_GETADDRINFO_A -DNN_HAVE_SOCKETPAIR -DNN_HAVE_SEMAPHORE -DNN_HAVE_GCC_ATOMIC_BUILTINS -DNN_HAVE_MSG_CONTROL -D_GNU_SOURCE

INCLUDES= -I.
LIBS_INCLUDE = -L./
LIBS=-lpthread -lm 

COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

TARGET=libnanomsg.so

all: $(TARGET)

LIBOBJS_core = core/ep.o \
			   core/epbase.o \
			   core/global.o \
			   core/pipe.o \
			   core/poll.o \
			   core/sock.o \
			   core/sockbase.o \
			   core/symbol.o

LIBOBJS_aio = aio/ctx.o \
			  aio/fsm.o \
			  aio/pool.o \
			  aio/poller.o \
			  aio/timer.o \
			  aio/timerset.o \
			  aio/usock.o \
			  aio/worker.o 

		
LIBOBJS_utils = utils/alloc.o \
				utils/atomic.o \
				utils/chunk.o \
				utils/chunkref.o \
				utils/clock.o \
				utils/closefd.o \
				utils/efd.o \
				utils/err.o \
				utils/glock.o \
				utils/hash.o \
				utils/list.o \
				utils/msg.o \
				utils/mutex.o \
				utils/queue.o \
				utils/random.o \
				utils/sem.o \
				utils/sleep.o  \
				utils/thread.o  \
				utils/wire.o

LIBOBJS_devices = devices/device.o devices/tcpmuxd.o

LIBOBJS_protocols = protocols/bus/bus.o \
					protocols/bus/xbus.o \
					protocols/utils/dist.o \
					protocols/utils/excl.o \
					protocols/utils/fq.o \
					protocols/utils/lb.o \
					protocols/utils/priolist.o \
					protocols/pipeline/push.o \
					protocols/pipeline/pull.o \
					protocols/pipeline/xpull.o \
					protocols/pipeline/xpush.o \
					protocols/pair/pair.o \
					protocols/pair/xpair.o \
					protocols/pubsub/pub.o \
					protocols/pubsub/sub.o \
					protocols/pubsub/trie.o \
					protocols/pubsub/xpub.o \
					protocols/pubsub/xsub.o \
					protocols/reqrep/req.o \
					protocols/reqrep/rep.o \
					protocols/reqrep/task.o \
					protocols/reqrep/xrep.o \
					protocols/survey/respondent.o \
					protocols/survey/surveyor.o \
					protocols/survey/xrespondent.o \
					protocols/survey/xsurveyor.o

LIBOBJS_transports = transports/utils/backoff.o \
					 transports/utils/dns.o \
					 transports/utils/iface.o \
					 transports/utils/literal.o \
					 transports/utils/port.o \
					 transports/utils/streamhdr.o \
					 transports/utils/base64.o \
					 transports/inproc/binproc.o \
					 transports/inproc/cinproc.o \
					 transports/inproc/inproc.o \
					 transports/inproc/ins.o \
					 transports/inproc/msgqueue.o \
					 transports/inproc/sinproc.o \
					 transports/ipc/aipc.o \
					 transports/ipc/bipc.o \
					 transports/ipc/cipc.o \
					 transports/ipc/ipc.o \
					 transports/ipc/sipc.o \
					 transports/tcp/atcp.o \
					 transports/tcp/btcp.o \
					 transports/tcp/ctcp.o \
					 transports/tcp/stcp.o \
					 transports/tcp/tcp.o \
					 transports/tcpmux/atcpmux.o \
					 transports/tcpmux/btcpmux.o \
					 transports/tcpmux/ctcpmux.o \
					 transports/tcpmux/stcpmux.o \
					 transports/tcpmux/tcpmux.o \
					 transports/ws/aws.o \
					 transports/ws/bws.o \
					 transports/ws/cws.o \
					 transports/ws/sws.o \
					 transports/ws/ws_handshake.o \
					 transports/ws/sha1.o 

LIBOBJS=$(LIBOBJS_core) $(LIBOBJS_aio) $(LIBOBJS_utils) $(LIBOBJS_devices) $(LIBOBJS_protocols) $(LIBOBJS_transports)

libnanomsg.so:$(LIBOBJS)
	$(CC) $(LIBS) -shared -o $@ $^

clean:
	-@rm -rf $(TARGET) *.o $(LIBOBJS)
