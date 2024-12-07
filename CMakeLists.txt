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
    "Include/AxonEngine.h" "Include/AxonEngine.cpp" 
    "Include/AxonError.h" "Include/AxonError.cpp" 
    "Include/AxonTypes.h" "Include/AxonTypes.cpp"
)

add_executable(AxonDockertestServer "tests/docker-test/entrypoint/server.cpp" ${MASTER_INCLUDE})
add_executable(AxonDockertestClient "tests/docker-test/entrypoint/client.cpp" ${MASTER_INCLUDE})
add_library(AxonServerLib SHARED ${AXON_ENGINE} ${AXON_SERVER} ${MASTER_INCLUDE})
add_library(AxonClientLib SHARED ${AXON_ENGINE} ${AXON_CLIENT} ${MASTER_INCLUDE})

target_link_libraries(AxonDockertestServer AxonServerLib)
target_link_libraries(AxonDockertestClient AxonClientLib)

target_compile_definitions(AxonDockertestServer PUBLIC AXON_SERVER)
target_compile_definitions(AxonServerLib PUBLIC AXON_SERVER_LIB)
target_compile_definitions(AxonDockertestClient PUBLIC AXON_CLIENT)

add_custom_command(TARGET AxonDockertestServer POST_BUILD COMMAND echo "Post build step")

if(WIN32)
  target_link_libraries(AxonDockertestServer ws2_32)
  target_link_libraries(AxonDockertestClient ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonDockertestServer PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonDockertestClient PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonServerLib PROPERTY CXX_STANDARD 20)
  set_property(TARGET AxonClientLib PROPERTY CXX_STANDARD 20)
endif()

enable_testing()

set(TEST_SERIALIZATION
    "tests/test_serialization.cxx"
)

add_executable(TestSerialization ${TEST_SERIALIZATION} ${AXON_ENGINE})

add_test(NAME TestSerialization COMMAND $<TARGET_FILE:TestSerialization>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestSerialization PROPERTY CXX_STANDARD 20)
endif()
