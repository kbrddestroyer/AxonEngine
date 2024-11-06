#pragma once

#include <Axon.h>


#if defined(WINDOWS_PLATFORM)
#if defined(AXON_SERVER)
#include <server/backends/win/winserver.h>
#elif defined(AXON_CLIENT)
#include <client/backends/win/winclient.h>
#endif

#elif defined(UNIX_PLATFORM)
#if defined(AXON_SERVER)
#include <server/backends/unix/unixserver.h>
#elif defined(AXON_CLIENT)
#include <client/backends/unix/unixclient.h>
#endif
#endif
