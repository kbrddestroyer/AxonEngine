cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.basics.basic-axon-networking")

add_executable(BasicAxonClient "client.cpp")
add_executable(BasicAxonServer "server.cpp")

target_link_libraries(BasicAxonClient NetworkingLib)
target_link_libraries(BasicAxonServer NetworkingLib)

if(WIN32)
  target_link_libraries(BasicAxonClient ws2_32)
  target_link_libraries(BasicAxonServer ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasicAxonClient PROPERTY CXX_STANDARD 17)
  set_property(TARGET BasicAxonServer PROPERTY CXX_STANDARD 17)
endif()
