/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>
#include <string>

int main()
{
	SOCKADDR_IN_T socket;
	SOCKET_T client;
	const char* hostname = "localhost";
	uint32_t port = 10423;

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

	const char* message = "Hello World!";

	int code = send_message(message, strlen(message), &client, &socket);
	if (code < 0)
	{
		
		return WSAGetLastError();
	}

	std::cin.get();
	return 0;
}
