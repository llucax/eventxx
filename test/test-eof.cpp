/*
 * Compile with:
 * c++ -I/usr/local/include -o time-test time-test.c -L/usr/local/lib -levent
 *
 *
 * Wed 2006-12-27 - Modified by Leandro Lucarella <llucarella@integratech.com.ar>
 *
 *     Adapted to test the C++ inteface.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <event>

int test_okay = 1;
int called = 0;
event::dispatcher d;
event::cevent* ev;

void
read_cb(int fd, short event, void *arg)
{
	char buf[256];
	int len;

	len = read(fd, buf, sizeof(buf));

	printf("%s: read %d%s\n", __func__,
	    len, len ? "" : " - means EOF");

	if (len) {
		if (!called)
			d.add(*ev);
	} else if (called == 1)
		test_okay = 0;

	called++;
}

int
main (int argc, char **argv)
{
	char *test = "test string";
	int pair[2];

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1)
		return (1);

	write(pair[0], test, strlen(test)+1);
	shutdown(pair[0], SHUT_WR);

	/* Initalize one event */
	ev = new event::cevent(pair[1], EV_READ, read_cb, NULL);

	d.add(*ev);

	d.dispatch();

	delete ev;

	return (test_okay);
}

