/*
*	Can be used as template for any other backends
* Created: 06.02.25
*/

#ifndef BASIC_NETWORKING_H
#define BASIC_NETWORKING_H

#include "master_include.h"
#include <stdint.h>

#define SUCCESS 0

#define ERR_WININIT_FAIL		1
#define ERR_INVALID				2
#define ERR_GETADDRINFO_FAIL	3


uint8_t initialize(SOCKADDR_IN_T* server, const char* hostname, uint32_t port);

// bool send_message(const char*);
// void listen();

// void notify_on_message(const char*);

#endif
/* basic_networking.h */