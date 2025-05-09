project ("AxonEngine.examples.basics.basic-axon-networking")

add_compile_definitions(TEST_MODE)

add_executable(BasicAxonClient "client.cpp")
add_executable(BasicAxonServer "server.cpp")

add_custom_command(TARGET BasicAxonClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:networking>
        $<TARGET_FILE_DIR:BasicAxonClient>
)

add_custom_command(TARGET BasicAxonServer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:networking>
        $<TARGET_FILE_DIR:BasicAxonServer>
)

target_include_directories(BasicAxonClient PUBLIC ${CMAKE_HOME_DIRECTORY}/libraries)
target_include_directories(BasicAxonServer PUBLIC ${CMAKE_HOME_DIRECTORY}/libraries)
target_link_libraries(BasicAxonClient networking)
target_link_libraries(BasicAxonServer networking)

if(WIN32)
  target_link_libraries(BasicAxonClient ws2_32)
  target_link_libraries(BasicAxonServer ws2_32)
endif(WIN32)
