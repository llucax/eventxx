#include <eventxx>
#include <iostream>
#include <map>
#include <csignal>
#include <cstdlib>

using eventxx::dispatcher;

#define OSASSERT(func, ...) \
	if (func(__VA_ARGS__) == -1) { perror(#func "()"); std::exit(1); }

struct handler
{
	dispatcher& d;
	int fds[4];
	std::map< int, eventxx::basic_event* > events;
	handler(dispatcher& d): d(d)
	{
		OSASSERT(pipe, fds);
		OSASSERT(pipe, fds+2);
	}
	void add(eventxx::basic_event& e)
	{
		events[e.fd()] = &e;
	}
	void operator() (int fd, eventxx::type event)
	{
		char buf[7];
		OSASSERT(read, fd, buf, 7);
		std::cout << "Read from fd " << fd << ": " << buf << "\n";
		d.del(*events[fd]);
	}
};

int main()
{
	dispatcher d(2);
	handler h(d);
	OSASSERT(write, h.fds[1], "hola 1", 7);
	OSASSERT(write, h.fds[3], "hola 2", 7);
	eventxx::event< handler > e1(h.fds[0], eventxx::READ, h);
	eventxx::event< handler > e2(h.fds[2], eventxx::READ, h);
	h.add(e1);
	h.add(e2);
	d.add(e1, 1);
	d.add(e2, 0);
	d.dispatch();
	return 0;
}

