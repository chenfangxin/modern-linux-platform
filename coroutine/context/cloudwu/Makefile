all : main

main : main.c coroutine.c
	gcc -g -Wall -o $@ $^

clean :
	- rm -f *.o *.tmp
	- rm -f main
