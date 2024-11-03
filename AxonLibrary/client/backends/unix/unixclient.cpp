#include "unixclient.h"

#if defined(__unix__) || __APPLE__

#include <string>

Axon::Backends::Unix::UnixUDPClient::UnixUDPClient(char *hostname, Axon::Connection::AXON_PORT port) :
    Axon::Connection::ClientConnectionHandler(hostname, port) {
    sockfd = 0;
    server = {};
}


bool Axon::Backends::Unix::UnixUDPClient::Initialize() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        throw AxonError(Error::AxonErrorCode::INTERNAL_ERROR);
    }

	addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	addrinfo* res;

	if (getaddrinfo(hostname, std::to_string(port).c_str(), &hints, &res) != 0)
	{
		return false;
	}

	memcpy(&server, (struct sockaddr_in* ) res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);

	printf("%s\n", inet_ntoa(server.sin_addr));

    return true;
}

void Axon::Backends::Unix::UnixUDPClient::SendUDPMessage(char* serialized, size_t size)
{
    sendto(sockfd, serialized, size, MSG_CTRUNC, (struct sockaddr*) &server, sizeof(server));
}

Axon::Backends::Unix::UnixUDPClient::~UnixUDPClient() {
    close(sockfd);
}
#endif
