CC=gcc
CFLAGS = -g
CFLAGS += -fPIC -Wall -Wunused-function -Wextra
INCLUDES= -I.
LIBS=-lpthread -lm

TARGET=demo libev.so

all: $(TARGET)
LIBOBJS=ev.o event.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

libev.so:$(LIBOBJS)
	$(CC) $(LIBS) -shared -o $@ $^

demo: demo.o
	$(CC) $(LIBS) -lev -shared -o $@ $^

clean:
	-@rm -rf $(TARGET) *.o
