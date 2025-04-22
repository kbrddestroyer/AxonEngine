cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.axon-api.axon-networking")

add_compile_definitions(TEST_MODE)

add_executable(AxonAPIClient "client.cpp")
add_executable(AxonAPIServer "server.cpp")

target_link_libraries(AxonAPIClient networking)
target_link_libraries(AxonAPIServer networking)

if(WIN32)
  target_link_libraries(AxonAPIClient ws2_32)
  target_link_libraries(AxonAPIServer ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AxonAPIClient PROPERTY CXX_STANDARD 17)
  set_property(TARGET AxonAPIServer PROPERTY CXX_STANDARD 17)
endif()
