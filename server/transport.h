#pragma once

#ifdef __unix__
#include <netinet/in.h>
#define UNIX
#endif

#ifdef _WIN32 || _WIN64
#include <winsock2.h>
#define WINDOWS
#endif


class Transport
{
private:

};