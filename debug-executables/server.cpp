#define AXON_SERVER

#include <AxonEngine.h>
#include <iostream>

using namespace Axon;

int main()
{
	Backends::Windows::WinUDPSocket socket = Backends::Windows::WinUDPSocket("127.0.0.1", 10243);



	if (socket.Startup())
	{
		std::cout << "Started!" << std::endl;
		socket.Listen();
	}

	return 0;
}
