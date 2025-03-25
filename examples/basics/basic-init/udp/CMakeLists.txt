cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.basics.udp.basic-init")

add_executable(BasicUDPClient "client.cpp" ${BACKEND_SOURCES})
add_executable(BasicUDPServer "server.cpp" ${BACKEND_SOURCES})

target_include_directories(BasicUDPClient PUBLIC ${CMAKE_HOME_DIRECTORY}/common)
target_include_directories(BasicUDPServer PUBLIC ${CMAKE_HOME_DIRECTORY}/common)

if(WIN32)
  target_link_libraries(BasicUDPClient ws2_32)
  target_link_libraries(BasicUDPServer ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasicUDPClient PROPERTY CXX_STANDARD 17)
  set_property(TARGET BasicUDPServer PROPERTY CXX_STANDARD 17)
endif()
