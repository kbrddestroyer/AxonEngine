cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.tests.gtest")

# TEST FILES
find_package(GTest REQUIRED)
enable_testing()

set (LIST_LIBRARIES networking GTest::gtest GTest::gtest_main)

list_all_tests("test_*.cpp" ${LIST_LIBRARIES})

include_directories("../../libraries")
