#define AXON_SERVER

#include <Axon.h>
#include <iostream>

using namespace Axon;

int main()
{
	Backends::Windows::WinUDPConnectionHandler socket = Backends::Windows::WinUDPConnectionHandler(10243);
	if (socket.Initialize())
	{
		std::cout << "Started!" << std::endl;
		socket.Start();
	}

	return 0;
}
