TARGETS := switch addrlabels

switch_OBJECTS = switch.o
addrlabel_OBJECTS = addrlabels.o

CFLAGS = -g -Wall -Wuninitialized -Werror
CC = gcc
INCLUDES=-I./
COMPILE = $(CC) $(CFLAGS) $(INCLUDES)
LINK = $(COMPILE) -o $@

all: $(TARGETS)

switch: $(switch_OBJECTS)
	$(LINK) $(switch_OBJECTS)

addrlabels: $(addrlabel_OBJECTS)
	$(LINK) $(addrlabel_OBJECTS)

switch.o: switch.c
	$(COMPILE) -c -o $@ $<

addrlabels.o: addrlabels.c
	$(COMPILE) -c -o $@ $<

clean:
	- rm -f *.o *.tmp
	- rm -f $(TARGETS)
