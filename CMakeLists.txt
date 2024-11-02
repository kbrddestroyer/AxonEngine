cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")
include("CTest")

include_directories("server")
include_directories("client")
include_directories("common")

set(AXON_ENGINE 
    "AxonEngine.h" "AxonEngine.cpp" 
    "common/AxonError.h" "common/AxonError.cpp" 
    "common/AxonTypes.h" "common/AxonTypes.cpp"
    "common/serialization/serialization.h" "common/serialization/serialization.c"
)
set(AXON_SERVER
    "server/server_connection.h" "server/server_connection.cpp"
    "server/AxonServer.h" "server/AxonServer.cpp"
    "server/backends/win/wintransport.h" "server/backends/win/wintransport.cpp"
    "server/backends/unix/unixtransport.h" "server/backends/unix/unixtransport.cpp"
    "server/server_constants.h"
)
set(AXON_CLIENT
    "client/backends/win/winclient.h" "client/backends/win/winclient.cpp"
    "client/client_connection.h" "client/client_connection.cpp"
    "client/backends/unix/unixclient.h" "client/backends/unix/unixclient.cpp"
)

set(MASTER_INCLUDE "Axon.h")

add_executable(AxonServer "debug-executables/server.cpp" "Axon.h")
add_executable(AxonClient "debug-executables/client.cpp" ${AXON_ENGINE} ${AXON_CLIENT} ${MASTER_INCLUDE})

add_library(AxonServerLib SHARED ${AXON_ENGINE} ${AXON_SERVER} ${MASTER_INCLUDE})
target_link_libraries(AxonServer PRIVATE AxonServerLib)

target_compile_definitions(AxonServer PUBLIC AXON_SERVER)
target_compile_definitions(AxonClient PUBLIC AXON_CLIENT)

target_include_directories(AxonServer PUBLIC .)
target_include_directories(AxonClient PUBLIC .)
target_include_directories(AxonServerLib PUBLIC .)

if(WIN32)
  target_link_libraries(AxonServer PRIVATE ws2_32)
  target_link_libraries(AxonClient ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonServer PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonClient PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonServerLib PROPERTY CXX_STANDARD 20)
endif()

# Tests
enable_testing()

set(TEST_SERIALIZATION
    "tests/test_serialization.cxx"
)

add_executable(TestSerialization ${TEST_SERIALIZATION} ${AXON_ENGINE})

add_test(NAME TestSerialization COMMAND $<TARGET_FILE:TestSerialization>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestSerialization PROPERTY CXX_STANDARD 20)
endif()