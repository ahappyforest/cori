#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ev.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "tap.h"

static void _signal_cb (struct ev_loop *loop, struct ev_signal *w, int revents)
{
	int signo;

	signo = (long int)(w->data);

	printf("Recv Signal => ");
	switch(signo) {
	case SIGINT:
		printf("SIGINT\n");
		printf("Bye.\n");
		ev_signal_stop(loop, w);
		ev_break(loop, EVBREAK_ALL);
		break;
	case SIGPIPE:
		printf("SIGPIPE\n");
		break;
	case SIGTERM:
		printf("SIGTERM\n");
		break;
	case SIGUSR1:
		printf("SIGUSR1\n");
		break;
	case SIGUSR2:
		printf("SIGUSR2\n");
		break;
	case SIGSEGV:
		printf("SIGSEGV\n");
		break;
	case SIGBUS:
		printf("SIGBUS\n");
		break;
	case SIGHUP:
		printf("SIGHUP\n");
		break;
	case SIGKILL:
		printf("SIGKILL\n");
		break;
	case SIGSTOP:
		printf("SIGSTOP\n");
		break;
	}
}

static int l_run(lua_State *L)
{
	int tapfd;

	struct ev_loop *loop = EV_DEFAULT;
	struct ev_signal signal_watcher_int  = {.data = (void *)SIGINT  };
	struct ev_signal signal_watcher_pipe = {.data = (void *)SIGPIPE };
	struct ev_signal signal_watcher_term = {.data = (void *)SIGTERM };
	struct ev_signal signal_watcher_usr1 = {.data = (void *)SIGUSR1 };
	struct ev_signal signal_watcher_usr2 = {.data = (void *)SIGUSR2 };
	struct ev_signal signal_watcher_segv = {.data = (void *)SIGSEGV };
	struct ev_signal signal_watcher_bus  = {.data = (void *)SIGBUS  };
	struct ev_signal signal_watcher_hup  = {.data = (void *)SIGHUP  };
	struct ev_signal signal_watcher_kill = {.data = (void *)SIGKILL };
	struct ev_signal signal_watcher_stop = {.data = (void *)SIGSTOP };

	ev_signal_init(&signal_watcher_int, _signal_cb, SIGINT);
	ev_signal_init(&signal_watcher_pipe, _signal_cb, SIGPIPE);
	ev_signal_init(&signal_watcher_term, _signal_cb, SIGTERM);
	ev_signal_init(&signal_watcher_usr1, _signal_cb, SIGUSR1);
	ev_signal_init(&signal_watcher_usr2, _signal_cb, SIGUSR2);
	ev_signal_init(&signal_watcher_segv, _signal_cb, SIGSEGV);
	ev_signal_init(&signal_watcher_bus, _signal_cb, SIGBUS);
	ev_signal_init(&signal_watcher_hup, _signal_cb, SIGHUP);
	ev_signal_init(&signal_watcher_kill, _signal_cb, SIGKILL);
	ev_signal_init(&signal_watcher_stop, _signal_cb, SIGSTOP);

	ev_signal_start(loop, &signal_watcher_int);
	ev_signal_start(loop, &signal_watcher_pipe);
	ev_signal_start(loop, &signal_watcher_term);
	ev_signal_start(loop, &signal_watcher_usr1);
	ev_signal_start(loop, &signal_watcher_usr2);
	ev_signal_start(loop, &signal_watcher_segv);
	ev_signal_start(loop, &signal_watcher_bus);
	ev_signal_start(loop, &signal_watcher_hup);
	ev_signal_start(loop, &signal_watcher_kill);
	ev_signal_start(loop, &signal_watcher_stop);

	tapfd = tun_alloc("tap0");
	assert(tapfd > 0);
	tap_config("tap0", "192.168.100.1", "255.255.255.0");

	ev_run(loop, 0);

	ev_default_destroy();

	return 0;
}

int luaopen_cori_core(lua_State *L)
{
	struct luaL_Reg main_func[] = {
		{"run", l_run},
		{NULL, NULL},
	};

	luaL_register(L, "cori_core", main_func);
	return 1;
}
