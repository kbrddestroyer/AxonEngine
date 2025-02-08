/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>


int main()
{
	SOCKADDR_IN_T server;
	SOCKET_T server_socket;
	uint32_t port = 10423;

	std::cout << "Starting server socket initialization..." << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = initializeServerSocket(&server, &server_socket, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;

	char message_buffer[256] = {};

	SOCKADDR_IN_T from;
	
	while (true)
	{
		if (int code = recv_message((char**) message_buffer, 256, server_socket, &from) > 0)
		{
			std::cout << message_buffer << std::endl;
		}

		int lastError = WSAGetLastError();

		if (lastError > 0)
		{
			std::cout << lastError << std::endl;
		}

		Sleep(100);
	}
	return 0;
}
