function(add_all_subdirs)
    file(GLOB V_GLOB LIST_DIRECTORIES true "*")
    foreach(item ${V_GLOB})
        if(IS_DIRECTORY ${item})
            FILE(GLOB IS_CMAKE "${item}/CMakeLists.txt")
            if (IS_CMAKE)
                add_subdirectory(${item})
            endif()
        endif()
    endforeach()
endfunction()

function(list_all_tests_separate name_pattern)
    file (GLOB_RECURSE TEST_SOURCES ${name_pattern})

    foreach (TEST_SRC ${TEST_SOURCES})
        # Get name to generate target name (e.g. test_something - TEST_SOMETHING)

        get_filename_component(FILENAME ${TEST_SRC} NAME_WE)
        string(TOUPPER ${FILENAME} FILENAME)

        add_executable(${FILENAME}_TARGET ${TEST_SRC})
        add_test(NAME ${FILENAME} COMMAND $<TARGET_FILE:${FILENAME}_TARGET>)
        target_link_libraries(${FILENAME}_TARGET PRIVATE ${ARGN})

        add_custom_command(TARGET ${FILENAME}_TARGET POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${FILENAME}_TARGET> $<TARGET_RUNTIME_DLLS:${FILENAME}_TARGET>
                COMMAND_EXPAND_LISTS
        )

        message ("Loaded test sequence: ${FILENAME}")
    endforeach ()
endfunction()

function(list_all_tests name_pattern)
    file (GLOB_RECURSE TEST_SOURCES ${name_pattern})
    get_filename_component(FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
    string (TOUPPER ${FOLDER} FOLDER)

    add_executable(TEST_${FOLDER}_TARGET ${TEST_SOURCES} "entry.cpp")
    add_test(NAME TEST_${FOLDER} COMMAND $<TARGET_FILE:TEST_${FOLDER}_TARGET>)
    target_link_libraries(TEST_${FOLDER}_TARGET PRIVATE ${ARGN})

    add_custom_command(TARGET TEST_${FOLDER}_TARGET POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:TEST_${FOLDER}_TARGET> $<TARGET_RUNTIME_DLLS:TEST_${FOLDER}_TARGET>
            COMMAND_EXPAND_LISTS
    )

    message ("Loaded test folder: TEST_${FOLDER}")
endfunction()