cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")

include_directories("server")
include_directories("client")
include_directories("common")

set(AXON_ENGINE "AxonEngine.h" "AxonEngine.cpp" "common/AxonError.h" "common/AxonError.cpp")
set(AXON_SERVER "server/server.h" "server/server.cpp" "server/transport.h" "server/transport.cpp" "server/backends/win/wintransport.h" "server/backends/win/wintransport.cpp")
set(AXON_CLIENT "client/backends/win/winclient.h" "client/backends/win/winclient.cpp")

add_executable(AxonEngine "debug-executables/server.cpp" ${AXON_ENGINE} ${AXON_SERVER})
add_executable(AxonClient "debug-executables/client.cpp" ${AXON_ENGINE} ${AXON_CLIENT})

target_include_directories(AxonEngine PUBLIC .)
target_include_directories(AxonClient PUBLIC .)

target_sources(AxonEngine PUBLIC "debug-executables/server.cpp")
target_sources(AxonClient PUBLIC "debug-executables/client.cpp")

if(WIN32)
  target_link_libraries(AxonEngine ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonEngine PROPERTY CXX_STANDARD 20)
endif()
