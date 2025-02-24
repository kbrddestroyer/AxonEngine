cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.tests.basic_checks")
include ("CTest")

# TEST FILES
enable_testing()

add_executable(TEST_SERIALIZATION_CXX "test_serialization.cxx" ${SERIALIZATION})
add_executable(TEST_EVENT_SYSTEM_CXX "test_event_system.cxx" ${AXON_MASTER})
add_test(NAME TEST_SERIALIZATION COMMAND $<TARGET_FILE:TEST_SERIALIZATION_CXX>)
add_test(NAME TEST_EVENT_SYSTEM COMMAND $<TARGET_FILE:TEST_EVENT_SYSTEM_CXX>)
