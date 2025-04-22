cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include_directories(${CMAKE_HOME_DIRECTORY}/libraries)

project ("AxonEngine.tests")
include ("CTest")

file(GLOB_RECURSE V_GLOB LIST_DIRECTORIES true "*")

message(STATUS "Searching for build config in ${V_GLOB}")

add_all_subdirs()
