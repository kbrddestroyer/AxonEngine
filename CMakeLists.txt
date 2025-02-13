cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine")
include("CTest")

include_directories(.)

# Common backends

add_subdirectory(common)

set (BACKEND_SOURCES
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/basic_networking.h
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/basic_networking.c
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/master_include.h
	${CMAKE_HOME_DIRECTORY}/backends/backend.h
)

set (EVENT_SYSTEM
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.cpp
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.hpp
)

# Examples
add_subdirectory(examples)
# Unit tests
add_subdirectory(tests)
