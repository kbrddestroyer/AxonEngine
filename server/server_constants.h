#pragma once

#define SERVER_PACKAGE_MAXSIZE	1024
#define USE_DEBUG_MESSAGES

#ifdef USE_DEBUG_MESSAGES
#include <iostream>

#define DEBUG(message) std::cerr << message << std::endl
#endif
