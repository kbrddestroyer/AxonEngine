#ifndef AXON_SERVER
#define AXON_SERVER
#endif

#include <Axon.h>
#include <AxonServer.h>

#include <iostream>

using namespace Axon;

int main()
{
    Axon::Connection::ServerConnectionHandler* connection;
#if defined(_WIN32)
	connection = new Backends::Windows::WinUDPConnectionHandler(10243);
#elif defined(__unix__) || __APPLE__
    connection = new Axon::Backends::Unix::UnixUDPConnectionHandler(10243);
#endif

    std::cout << "Started!" << std::endl;
    connection->Start();

    delete connection;
	return 0;
}
