cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")

include_directories("server")
include_directories("client")
include_directories("common")

set(AXON_ENGINE "AxonEngine.h" "AxonEngine.cpp" "common/AxonError.h" "common/AxonError.cpp" "common/AxonTypes.h")
set(AXON_SERVER "server/server_connection.h" "server/server_connection.cpp" "server/AxonServer.h" "server/AxonServer.cpp" "server/backends/win/wintransport.h" "server/backends/win/wintransport.cpp")
set(AXON_CLIENT "client/backends/win/winclient.h" "client/backends/win/winclient.cpp")

set(MASTER_INCLUDE "Axon.h")

add_executable(AxonServer "debug-executables/server.cpp" ${AXON_ENGINE} ${AXON_SERVER} ${MASTER_INCLUDE})
add_executable(AxonClient "debug-executables/client.cpp" ${AXON_ENGINE} ${AXON_CLIENT} ${MASTER_INCLUDE})

target_compile_definitions(AxonServer PUBLIC AXON_SERVER)
target_compile_definitions(AxonClient PUBLIC AXON_CLIENT)

target_include_directories(AxonServer PUBLIC .)
target_include_directories(AxonClient PUBLIC .)

if(WIN32)
  target_link_libraries(AxonServer ws2_32)
  target_link_libraries(AxonClient ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonServer PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonClient PROPERTY CXX_STANDARD 20)
endif()
