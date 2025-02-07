/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>


int main()
{
	SOCKADDR_IN_T socket;
	SOCKET_T client;
	const char* hostname = "localhost";
	uint32_t port = 8022;

	std::cout << "Starting client socket initialization..." << std::endl;
	std::cout << "HOSTNAME: " << hostname << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = initializeClientConnection(&socket, &client, hostname, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;

	std::cout << "Press any key...";
	std::getchar();		// Wait for input

	SOCKADDR_IN_T server;
	SOCKET_T server_socket;

	std::cout << "Starting server socket initialization..." << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = initializeServerSocket(&server, &server_socket, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;
	std::getchar();

	return 0;
}