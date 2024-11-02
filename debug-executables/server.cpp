#ifndef AXON_SERVER
#define AXON_SERVER
#endif

#include <Axon.h>
#include <iostream>
#include <cstdint>

using namespace Axon;

int main()
{
    const uint16_t port = 10243;

    Axon::Connection::ServerConnectionHandler* connection;
#if defined(_WIN32)
	connection = new Backends::Windows::WinUDPConnectionHandler(port);
#elif defined(__unix__) || __APPLE__
    connection = new Axon::Backends::Unix::UnixUDPConnectionHandler(port);
#endif

    std::cout << "Server Started!" << std::endl;
    connection->Start();

    delete connection;
	return 0;
}
