TARGETS := demo

target_OBJECTS = objs/demo.o

CFLAGS = -g -Wall -rdynamic
CC = gcc
NM = nm
INCLUDES= -I../include/ 
LIBS = -ldl -lm -lpthread
COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

all: $(TARGETS)

demo: $(target_OBJECTS) 
	$(LINK) $(target_OBJECTS) $(LIBS) $(LIBS_INCLUDE)

objs/demo.o: demo.c
	$(COMPILE) -c -o $@ $<

clean:
	- rm -f objs/*.o .tmp*
	- rm -f $(TARGETS)
