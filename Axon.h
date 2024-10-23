#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

#if defined(AXON_SERVER)
#include <server/backends/win/wintransport.h>
#elif defined(AXON_CLIENT)
#include <client/backends/win/winclient.h>
#endif

#endif
