cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")
include("CTest")

include_directories("Include")
include_directories("AxonLibrary")
include_directories("AxonLibrary/server")
include_directories("AxonLibrary/client")
include_directories("AxonLibrary/common")

set(AXON_ENGINE 
    "Include/AxonConnectionHandler.h"
    "Include/AxonEngine.h" "Include/AxonEngine.cpp" 
    "AxonLibrary/AxonBackend.h" 
    "Include/AxonError.h" "Include/AxonError.cpp" 
    "Include/AxonTypes.h" "Include/AxonTypes.cpp"
    "AxonLibrary/common/serialization/serialization.h" "AxonLibrary/common/serialization/serialization.c"
)
set(AXON_SERVER
    "AxonLibrary/server/server_connection.h" "AxonLibrary/server/server_connection.cpp"
    "AxonLibrary/server/AxonServer.h" "AxonLibrary/server/AxonServer.cpp"
    "AxonLibrary/server/backends/win/winserver.h" "AxonLibrary/server/backends/win/winserver.cpp"
    "AxonLibrary/server/backends/unix/unixserver.h" "AxonLibrary/server/backends/unix/unixserver.cpp"
    "AxonLibrary/server/server_constants.h"
)
set(AXON_CLIENT
    "AxonLibrary/client/backends/win/winclient.h" "AxonLibrary/client/backends/win/winclient.cpp"
    "AxonLibrary/client/client_connection.h" "AxonLibrary/client/client_connection.cpp"
    "AxonLibrary/client/backends/unix/unixclient.h" "AxonLibrary/client/backends/unix/unixclient.cpp"
    "AxonLibrary/client/AxonClient.h" "AxonLibrary/client/AxonClient.cpp"
)
set(MASTER_INCLUDE 
    "Include/Axon.h"
    "Include/AxonConnectionHandler.h"
    "Include/AxonEngine.h" "Include/AxonEngine.cpp" 
    "AxonLibrary/AxonBackend.h" 
    "Include/AxonError.h" "Include/AxonError.cpp" 
    "Include/AxonTypes.h" "Include/AxonTypes.cpp"
)

add_executable(AxonServer "debug-executables/server.cpp" ${MASTER_INCLUDE})
add_executable(AxonClient "debug-executables/client.cpp" ${MASTER_INCLUDE})
add_library(AxonServerLib SHARED ${AXON_ENGINE} ${AXON_SERVER} ${AXON_CLIENT} ${MASTER_INCLUDE})

target_link_libraries(AxonServer AxonServerLib)
target_link_libraries(AxonClient AxonServerLib)

target_compile_definitions(AxonServer PUBLIC AXON_SERVER)
target_compile_definitions(AxonServerLib PUBLIC AXON_SERVER_LIB)
target_compile_definitions(AxonClient PUBLIC AXON_CLIENT)

target_include_directories(AxonServer PUBLIC .)
target_include_directories(AxonClient PUBLIC .)
target_include_directories(AxonServerLib PUBLIC .)

add_custom_command(TARGET AxonServer POST_BUILD COMMAND echo "Post build step")

if(WIN32)
  target_link_libraries(AxonServer ws2_32)
  target_link_libraries(AxonClient ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonServer PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonClient PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonServerLib PROPERTY CXX_STANDARD 20)
endif()

enable_testing()

set(TEST_SERIALIZATION
    "tests/test_serialization.cxx"
)

add_executable(TestSerialization ${TEST_SERIALIZATION} ${AXON_ENGINE} "AxonLibrary/AxonBackend.h"  "Include/AxonConnectionHandler.h")

add_test(NAME TestSerialization COMMAND $<TARGET_FILE:TestSerialization>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestSerialization PROPERTY CXX_STANDARD 20)
endif()
