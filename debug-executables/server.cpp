#define AXON_SERVER

#include <Axon.h>
#include <iostream>

using namespace Axon;

int main()
{
	Backends::Windows::WinUDPConnectionHandler socket = Backends::Windows::WinUDPConnectionHandler(10243);
	if (socket.Startup())
	{
		std::cout << "Started!" << std::endl;
		socket.Listen();
	}

	return 0;
}
