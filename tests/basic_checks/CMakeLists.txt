cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.tests.basic_checks")
include ("CTest")

include_directories(${CMAKE_HOME_DIRECTORY}/common)

set (SERIALIZATION
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.h
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.c
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.hpp
)

set (EVENT_SYSTEM
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.cpp
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.hpp
)

# TEST FILES
enable_testing()

add_executable(TEST_SERIALIZATION_CXX "test_serialization.cxx" ${SERIALIZATION})
add_executable(TEST_EVENT_SYSTEM_CXX "test_event_system.cxx" ${EVENT_SYSTEM})
add_test(NAME TEST_SERIALIZATION COMMAND $<TARGET_FILE:TEST_SERIALIZATION_CXX>)
add_test(NAME TEST_EVENT_SYSTEM COMMAND $<TARGET_FILE:TEST_EVENT_SYSTEM_CXX>)
