#include <eventxx>
#include <iostream>
#include <csignal>

using eventxx::dispatcher;

struct handler
{
	dispatcher& d;
	int i;
	handler(dispatcher& d): d(d), i(0) {}
	void handle_event(int signum, eventxx::type event)
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

typedef eventxx::mem_cb< handler, void (handler::*)(int, eventxx::type) > cb_type;

int main()
{
	dispatcher d;
	handler h(d);
	cb_type cb(h, &handler::handle_event);
	eventxx::signal< cb_type > e(SIGINT, cb);
	d.add(e);
	d.dispatch();
	return 0;
}

