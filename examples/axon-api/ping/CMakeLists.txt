cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.axon-api.ping-example")

add_compile_definitions(TEST_MODE)

add_executable(PingClient "client.cpp")
add_executable(PingServer "server.cpp")

target_link_libraries(PingClient networking)
target_link_libraries(PingServer networking)

add_custom_command(TARGET PingClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:networking>
        $<TARGET_FILE_DIR:PingClient>
)
add_custom_command(TARGET PingServer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:networking>
        $<TARGET_FILE_DIR:PingServer>
)

if(WIN32)
  target_link_libraries(PingClient ws2_32)
  target_link_libraries(PingServer ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PingClient PROPERTY CXX_STANDARD 17)
  set_property(TARGET PingServer PROPERTY CXX_STANDARD 17)
endif()
