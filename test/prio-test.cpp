#include <eventxx>
#include <iostream>
#include <csignal>

using eventxx::dispatcher;

#define OSASSERT(func, ...) if (func(__VA_ARGS__) == -1) { perror(#func "()"); exit(1); }

struct handler
{
	dispatcher& d;
	int fds[4];
	handler(dispatcher& d): d(d)
	{
		OSASSERT(pipe, fds);
		OSASSERT(pipe, fds+2);
	}
	void operator() (int fd, short event)
	{
		char buf[7];
		OSASSERT(read, fd, buf, 7);
		std::cout << "Read from fd " << fd << ": " << buf << "\n";
		d.exit();
	}
};

int main()
{
	dispatcher d(2);
	handler h(d);
	OSASSERT(write, h.fds[1], "hola 1", 7);
	OSASSERT(write, h.fds[3], "hola 2", 7);
	eventxx::event< handler > e1(h.fds[0], EV_READ, h);
	eventxx::event< handler > e2(h.fds[2], EV_READ, h);
	d.add(e1, 1);
	d.add(e2, 0);
	d.dispatch();
	return 0;
}

