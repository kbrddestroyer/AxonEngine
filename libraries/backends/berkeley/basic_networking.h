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

#endif

/* basic_networking.h */
