/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>


int main()
{
	SOCKADDR_IN_T socket;

	const char* hostname = "localhost";
	uint32_t port = 8022;

	std::cout << "Starting socket initialization..." << std::endl;
	std::cout << "HOSTNAME: " << hostname << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = initialize(&socket, hostname, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;

	std::cout << "Press any key...";
	std::getchar();		// Wait for input
}