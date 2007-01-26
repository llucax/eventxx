#include <eventxx>
#include <iostream>
#include <csignal>

using eventxx::dispatcher;

struct handler
{
	dispatcher& d;
	int i;
	handler(dispatcher& d): d(d), i(0) {}
	void operator() (int signum, short event)
	{
		if (i < 5) std::cout << "keep going...\n";
		else
		{
			std::cout << "done!\n";
			d.exit();
		}
	}
};

void sighandler(int signum, short event, void* data)
{
	int& i = *static_cast< int* >(data);
	std::cout << ++i << " interrupts, ";
}

int main()
{
	dispatcher d;
	handler h(d);
	eventxx::csignal sigev(SIGINT, sighandler, &h.i);
	eventxx::signal< handler > e(SIGINT, h);
	d.add(sigev);
	d.add(e);
	d.dispatch();
	return 0;
}

