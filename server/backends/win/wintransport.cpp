#include "wintransport.h"
#include "wintransport.h"
#include <iostream>
#include <AxonEngine.h>

#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__NT__)
Axon::Backends::Windows::WinUDPConnectionHandler::WinUDPConnectionHandler(Axon::Connection::AXON_PORT port)
{
    this->port = port;
}

Axon::Backends::Windows::WinUDPConnectionHandler::~WinUDPConnectionHandler()
{
    closesocket(server_socket);
    WSACleanup();
}

bool Axon::Backends::Windows::WinUDPConnectionHandler::Initialize()
{
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        throw Axon::AxonError(Axon::Error::AxonErrorCode::INTERNAL_ERROR);
    }
    
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        return false;
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool Axon::Backends::Windows::WinUDPConnectionHandler::SendUserMessage(Axon::Connection::ServerUDPMessage message)
{
    size_t size;
    std::shared_ptr<char[]> serialized = message.payload.getSerializedData(size);
    SOCKADDR_IN client = connections[message.connectionID];

    delete[] message.payload.data;

    return sendto(server_socket, serialized.get(), size, 0, (SOCKADDR*)&client, sizeof(client)) != SOCKET_ERROR;
}

void Axon::Backends::Windows::WinUDPConnectionHandler::Listen()
{
    std::shared_ptr<char[]> buffer(new char[SERVER_PACKAGE_MAXSIZE]);

    SOCKADDR_IN client;
    int32_t len = sizeof(client);
    int32_t size;
    while (isRunning) {
        if ((size = recvfrom(server_socket, buffer.get(), SERVER_PACKAGE_MAXSIZE, 0, (SOCKADDR*)&client, &len)) != SOCKET_ERROR)
        {
            buffer[size] = 0;
            std::cout << "Recfrom: " << buffer << " | " << inet_ntoa(client.sin_addr) << std::endl;

            OnMessageRecieved(buffer, size, client);
        }
        else
        {
            std::cout << WSAGetLastError() << std::endl;
            isRunning = false;
        }
    }
}

void Axon::Backends::Windows::WinUDPConnectionHandler::OnMessageRecieved(std::shared_ptr<char[]> serialized, size_t size, SOCKADDR_IN client)
{
    Axon::Connection::ServerUDPMessage message;
    message.payload.setDeserialized(serialized, size);

    if (message.payload.size == sizeof(uint32_t))
    {
        // Server internal command ?
        uint32_t serverCommand = *(uint32_t*)(message.payload.data);

        switch (serverCommand)
        {
        case 1:
            // Connection dispatch
        {
            size_t connectionID = connections.size();
            connections.insert({ connectionID, client });

            Axon::Connection::ServerUDPMessage message;
            message.connectionID = connectionID;
            message.payload.data = new char[sizeof(connectionID)];
            
            uint32_t* data = (uint32_t*)message.payload.data;
            *data = connectionID;

            message.payload.size = sizeof(connectionID);
            message.payload.tag = 1;

            SendUserMessage(message);
            break;
        }
        case 2:
        {
            connections.erase(message.connectionID);
        }
        }

    }
}
#endif