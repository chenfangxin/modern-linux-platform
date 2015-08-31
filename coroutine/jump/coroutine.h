#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <setjmp.h>

typedef struct{
	jmp_buf callee_context;
	jmp_buf caller_context;
}coroutine;

typedef void (*func)(void *);

void start(coroutine *c, func f, void *arg, void *sp);
void yield(coroutine *c);
int next(coroutine *c);

#endif
