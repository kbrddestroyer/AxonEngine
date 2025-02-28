cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.axon-api.chat")

add_compile_definitions(TEST_MODE)

add_executable(ChatClient "client.cpp")
add_executable(ChatServer "server.cpp")

target_link_libraries(ChatClient NetworkingLib)
target_link_libraries(ChatServer NetworkingLib)

add_custom_command(TARGET ChatClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:NetworkingLib>
        $<TARGET_FILE_DIR:ChatClient>
)
add_custom_command(TARGET ChatServer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:NetworkingLib>
        $<TARGET_FILE_DIR:ChatServer>
)

if(WIN32)
  target_link_libraries(ChatClient ws2_32)
  target_link_libraries(ChatServer ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChatClient PROPERTY CXX_STANDARD 17)
  set_property(TARGET ChatServer PROPERTY CXX_STANDARD 17)
endif()
