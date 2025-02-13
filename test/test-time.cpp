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
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <eventxx>

int called = 0;

#define NEVENT	20000

eventxx::ctimer* ev[NEVENT];
eventxx::dispatcher d;

void
time_cb(int fd, short event, void *arg)
{
	called++;

	if (called < 10*NEVENT) {
		for (int i = 0; i < 10; i++) {
			eventxx::time tv(0, random() % 50000L);
			int j = random() % NEVENT;
			if (tv.usec() % 2)
				d.add(*ev[j], tv);
			else
				d.del(*ev[j]);
		}
	}
}

int
main (int argc, char **argv)
{
	for (int i = 0; i < NEVENT; i++) {
		/* Initalize one event */
		ev[i] = new eventxx::ctimer(time_cb, NULL);
		d.add(*ev[i], eventxx::time(0, random() % 50000L));
	}

	d.dispatch();

	return (called < NEVENT);
}

