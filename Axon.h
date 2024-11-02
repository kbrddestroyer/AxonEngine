#define WINDOWS_PLATFORM defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define UNIX_PLATFORM __APPLE__ || __APPLE_CC__ || defined(__unix__)

#if WINDOWS_PLATFORM
#if defined(AXON_SERVER)
#include <server/backends/win/wintransport.h>
#elif defined(AXON_CLIENT)
#include <client/backends/win/winclient.h>
#endif

#elif UNIX_PLATFORM
#if defined(AXON_SERVER)
#include <server/backends/unix/unixtransport.h>
#elif defined(AXON_CLIENT)
#include <client/backends/unix/unixclient.h>
#endif
#endif
