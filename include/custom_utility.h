#ifndef CUSTOM_UTILITY_H
#define CUSTOM_UTILITY_H

#ifdef __cpluslus
#include <cstdint>
#else
#include <stdint.h>
#endif
// Define alias for uint64_t to be used instead of size_t
//  as size_t may have different size on different machines (i.e. x86 vs x64)
typedef uint64_t size64_t;

#endif //CUSTOM_UTILITY_H
