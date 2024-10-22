cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")

include_directories("server")
include_directories("client")

add_executable (AxonEngine "entrypoint.cpp" "AxonEngine.h" "AxonEngine.cpp" "server/server.h" "server/server.cpp" "server/transport.h" "server/transport.cpp" "server/win/wintransport.h" "server/win/wintransport.cpp" "client/win/winclient.h" "client/win/winclient.cpp")
target_include_directories(AxonEngine PUBLIC .)

if(WIN32)
  target_link_libraries(AxonEngine ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonEngine PROPERTY CXX_STANDARD 20)
endif()
