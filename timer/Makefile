TARGETS := timer

target_OBJECTS = timer.o rte_timer.o wheel_timer.o \
				 min_heap_timer.o rbtree_timer.o rbtree.o\
				 skiplist_timer.o

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

rte_timer.o: rte_timer.c rte_timer.h
	$(COMPILE) -c -o $@ $<

wheel_timer.o: wheel_timer.c wheel_timer.h
	$(COMPILE) -c -o $@ $<

min_heap_timer.o: min_heap_timer.c min_heap_timer.h
	$(COMPILE) -c -o $@ $<

rbtree_timer.o: rbtree_timer.c rbtree_timer.h
	$(COMPILE) -c -o $@ $<

rbtree.o: rbtree.c rbtree.h
	$(COMPILE) -c -o $@ $<

skiplist_timer.o: skiplist_timer.c skiplist_timer.h
	$(COMPILE) -c -o $@ $<

clean:
	- rm -f *.o *.tmp
	- rm -f $(TARGETS)
