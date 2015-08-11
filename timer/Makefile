TARGETS := timer

target_OBJECTS = timer.o rte_timer.o wheel_timer.o \
				 min_heap_timer.o rbtree_timer.o

CFLAGS = -g -Wall -rdynamic
CC = gcc
NM = nm
INCLUDES= -I../include/  -I./
LIBS = -ldl -lm -lpthread
COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

all: $(TARGETS)

timer : $(target_OBJECTS)
	$(LINK) $(target_OBJECTS) $(LIBS) $(LIBS_INCLUDE)

timer.o: timer.c
	$(COMPILE) -c -o $@ $<

rte_timer.o: rte_timer.c
	$(COMPILE) -c -o $@ $<

wheel_timer.o: wheel_timer.c
	$(COMPILE) -c -o $@ $<

min_heap_timer.o: min_heap_timer.c
	$(COMPILE) -c -o $@ $<

rbtree_timer.o: rbtree_timer.c
	$(COMPILE) -c -o $@ $<

clean:
	- rm -f *.o *.tmp
	- rm -f $(TARGETS)
