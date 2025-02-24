#ifndef MASTER_INCLUDE_H
#define MASTER_INCLUDE_H

#pragma region ERROR_CODES
	#define ERR_WININIT_FAIL		1
	#define ERR_INVALID				2
	#define ERR_GETADDRINFO_FAIL	3
	#define ERR_COULD_NOT_BIND		4

	#define SUCCESS					0
#pragma endregion

/* unix */

#if defined(__unix__)
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#define SOCKET_HEAD_INIT 

#define ADDRINFO_T struct addrinfo
#define SOCKET_T int32_t
#define SOCKADDR_T struct sockaddr
#define SOCKADDR_IN_T struct sockaddr_in
#define CHECK_VALID(s) ((s) >= 0)
#define CLOSESOCKET(s) close(&s)

#define GET_SOCKET_ERROR() (errno)
#endif

/* windows */

#if defined(_WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")


#define SOCKET_HEAD_INIT 								\
{														\
	WSADATA ws;											\
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {			\
														\
			return ERR_WININIT_FAIL;					\
	}													\
}														\

#define SOCKET_T SOCKET
#define SOCKADDR_T SOCKADDR
#define SOCKADDR_IN_T SOCKADDR_IN
#define ADDRINFO_T ADDRINFO
#define CHECK_VALID(s) ((s) != SOCKET_ERROR)
#define CLOSESOCKET(s) closesocket(s)

#define GET_SOCKET_ERROR() (WSAGetLastError())

#endif

/* common */

#include <stdio.h>
#include <stdlib.h>

#endif