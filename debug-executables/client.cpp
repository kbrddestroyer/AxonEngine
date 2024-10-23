#define AXON_CLIENT

#include <Axon.h>
#include <iostream>

using namespace Axon;

int main()
{
	if (!Backends::Windows::WinUDPClient(10243).Startup())
	{
		std::cout << "Could not start" << std::endl;
	}
	else
	{
		std::cout << "Success!" << std::endl;
	}
	std::cin.get();
	return 0;
}
