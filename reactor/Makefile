CC=gcc
CFLAGS = -g
# CFLAGS += -fPIC -Wall -Wunused-function -Wextra
CFLAGS += -fPIC -Wunused-function -Wextra
INCLUDES= -I.
LIBS_INCLUDE = -L./
LIBS=-lpthread -lm

COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

TARGET=libev.so demo

all: $(TARGET)
LIBOBJS=ev.o event.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

libev.so:$(LIBOBJS)
	$(CC) $(LIBS) -shared -o $@ $^

demo: demo.o
	$(LINK) demo.o $(LIBS) $(LIBS_INCLUDE) -lev

clean:
	-@rm -rf $(TARGET) *.o
