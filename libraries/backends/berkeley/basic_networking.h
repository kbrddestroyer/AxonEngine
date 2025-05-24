/*
* Can be used as template for any other backends
* 
* Defines some low-level socket logic. Direct usage is not recommended
* Created: 06.02.25
*/

#ifndef BASIC_NETWORKING_H
#define BASIC_NETWORKING_H

#include "master_include.h"
#include "udp/udp_connection.h"
#include "tcp/tcp_connection.h"

struct Socket
{
	SOCKET_T		socket;
	SOCKADDR_IN_T	conn;
};

struct Header {
    size_t size = 0;
};

struct Footer {
    /* sizeof = 2 bytes */

    uint8_t partID = 0;
    uint8_t flags = 0;
};

static_assert(
        sizeof(Header) + sizeof(Footer) < SYNAPSE_MESSAGE_SIZE_MAX,
        "There's no space left on network package"
);

struct Body {
    const char serialized[SYNAPSE_MESSAGE_SIZE_MAX - sizeof(Header) - sizeof(Footer)] = { 0 };
};

struct NetworkMessage {
    Header      header;
    Body        body;
    Footer      footer;
};

#endif

/* basic_networking.h */
