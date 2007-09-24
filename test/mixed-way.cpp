#include <eventxx>
#include <iostream>
#include <csignal>

struct handler
{
	eventxx::dispatcher& d;
	int i;
	handler(eventxx::dispatcher& d): d(d), i(0) {}
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

void timer_handler(int, short, void*)
{
	std::cout << "Press Ctrl-C 5 times to quit.\n";
}

int main()
{
	eventxx::dispatcher d;
	handler h(d);
	eventxx::ctimer t(timer_handler);
	eventxx::signal< handler > e(SIGINT, h);
	d.add(t, eventxx::time(1)); // 1 second
	d.add(e);
	d.dispatch();
	return 0;
}

