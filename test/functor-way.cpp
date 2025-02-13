#include <eventxx>
#include <iostream>
#include <csignal>

using eventxx::dispatcher;

struct handler
{
	dispatcher& d;
	int i;
	handler(dispatcher& d): d(d), i(0) {}
	void operator() (int signum, eventxx::type event)
	{
		std::cout << ++i << " interrupts, ";
		if (i < 5) std::cout << "keep going...\n";
		else
		{
			std::cout << "done!\n";
			d.exit();
		}
	}
};

int main()
{
	dispatcher d;
	handler h(d);
	eventxx::signal< handler > e(SIGINT, h);
	d.add(e);
	d.dispatch();
	return 0;
}

