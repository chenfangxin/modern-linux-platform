TARGETS := demo

target_OBJECTS = demo.o coroutine.o
CFLAGS = -g -Wall
CC = gcc
INCLUDES=-I./
COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

all: $(TARGETS)

demo: $(target_OBJECTS)
	$(LINK) $(target_OBJECTS)

demo.o: demo.c
	$(COMPILE) -c -o $@ $<

coroutine.o: coroutine.c coroutine.h
	$(COMPILE) -c -o $@ $<

clean:
	- rm -f *.o *.tmp
	- rm -f $(TARGETS)
