#ifndef MASTER_INCLUDE_H
#define MASTER_INCLUDE_H

/* unix */

#if defined(__unix__)
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

#define SOCKET sockaddr
#define SOCKADD_IN_T sockaddr_in
#define CHECK_VALID(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)

#define GET_SOCKET_ERROR() (errno)
#endif

/* windows */

#if defined(_WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")

#define SOCKET_T SOCKET
#define SOCKADDR_IN_T SOCKADDR_IN
#define ADDRINFO_T ADDRINFO
#define CHECK_VALID(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)

#define GET_SOCKET_ERROR() (WSAGetLastError())

#endif

/* common */

#include <stdio.h>
#include <stdlib.h>

#endif