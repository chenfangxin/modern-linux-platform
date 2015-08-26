#include <ev.h>
#include <stdio.h>

ev_timer  timeout_watcher;
static void timeout_cb(EV_P_ ev_timer *w, int revents)
{
	puts("timeout");
	// ev_break(EV_A_ EVBREAK_ONE);
}

int main(void)
{
	struct ev_loop *loop=EV_DEFAULT;

	// ev_io_init();
	// ev_io_start();

	ev_timer_init(&timeout_watcher, timeout_cb, 5.5, 1);
	ev_timer_start(loop, &timeout_watcher);

	ev_run(loop, 0);

	return 0;
}
