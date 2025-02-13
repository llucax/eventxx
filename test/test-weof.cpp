/*
 * Compile with:
 * c++ -I/usr/local/include -o time-test time-test.cpp -L/usr/local/lib -levent
 *
 * Wed 2006-12-27 - Modified by Leandro Lucarella <llucax+eventxx@gmail.com>
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
#include <csignal>
#include <cerrno>

#include <eventxx>

typedef void (cb_t)(int, short);

int pair[2];
int test_okay = 1;
int called = 0;
eventxx::dispatcher d;
eventxx::event< cb_t >* ev;

void
write_cb(int fd, short event)
{
	const char* test = "test string";
	int len;

	len = write(fd, test, strlen(test) + 1);

	printf("%s: write %d%s\n", __func__,
	    len, len ? "" : " - means EOF");

	if (len > 0) {
		if (!called)
			d.add(*ev);
		close(pair[0]);
	} else if (called == 1)
		test_okay = 0;

	called++;
}

int
main (int argc, char **argv)
{
	if (signal(SIGPIPE, SIG_IGN) == SIG_IGN)
		return (1);

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1)
		return (1);

	/* Initalize one event */
	ev = new eventxx::event< cb_t >(pair[1], eventxx::WRITE, write_cb);

	d.add(*ev);

	d.dispatch();

	delete ev;

	return (test_okay);
}

