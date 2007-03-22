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
	eventxx::dispatcher d;
	handler h(d);
	eventxx::csignal sigev(SIGINT, sighandler, &h.i);
	eventxx::signal< handler > e(SIGINT, h);
	d.add(sigev);
	d.add(e);
	d.dispatch();
	return 0;
}

