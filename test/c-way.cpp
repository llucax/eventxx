#include <eventxx>
#include <iostream>
#include <csignal>

eventxx::dispatcher d;

void sighandler(int signum, short event, void* data)
{
	unsigned &i = *(unsigned*)data;
	std::cout << ++i << " interrupts, ";
	if (i < 5) std::cout << "keep going...\n";
	else
	{
		std::cout << "done!\n";
		d.exit();
	}
}

int main()
{
	int var = 0;
	eventxx::csignal sigev(SIGINT, sighandler, &var);
	d.add(sigev);
	d.dispatch();
	return 0;
}

