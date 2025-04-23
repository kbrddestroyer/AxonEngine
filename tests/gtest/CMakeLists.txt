cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.tests.gtest")

# TEST FILES
find_package(GTest REQUIRED)
enable_testing()

file (GLOB_RECURSE TEST_SOURCES "test_*.cpp")
message ("Loaded test files: ${TEST_SOURCES}")

set (LIST_LIBRARIES networking GTest::gtest GTest::gtest_main)

foreach (TEST_SRC ${TEST_SOURCES})
    get_filename_component(FILENAME ${TEST_SRC} NAME_WE)
    string(TOUPPER ${FILENAME} FILENAME)
    add_executable(${FILENAME}_CXX ${TEST_SRC})
    target_link_libraries(${FILENAME}_CXX PRIVATE ${LIST_LIBRARIES})
    add_test(NAME ${FILENAME} COMMAND $<TARGET_FILE:TEST_EVENT_SYSTEM_CXX>)

    message ("Loaded test sequence: ${FILENAME}")
endforeach ()

include_directories("../../libraries")
