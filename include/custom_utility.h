/**
 * Contains utility types, functions, etc.
 */

#ifndef CUSTOM_UTILITY_H
#define CUSTOM_UTILITY_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "common_macro.h"

/**
* Define alias for uint64_t to be used instead of size_t
* as size_t may have different size on different machines (i.e. x86 vs x64)
*/
typedef uint64_t size64_t;
struct Socket
{
    SOCKET_T		socket;
    SOCKADDR_IN_T	conn;
};

#endif //CUSTOM_UTILITY_H
