CC=gcc
CFLAGS = -g
CFLAGS += -fPIC -Wall -Wunused-function -Wextra
INCLUDES= -I.
LIBS=-lpthread -lm

LIB=libev.so

all: $(LIB)
LIBOBJS=ev.o event.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(LIB):$(LIBOBJS)
	$(CC) $(LIBS) -shared -o $@ $^

clean:
	-@rm -rf $(LIB) *.o
