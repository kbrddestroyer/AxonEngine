cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include_directories(${CMAKE_HOME_DIRECTORY}/common)

project ("AxonEngine.tests")
include ("CTest")

file(GLOB V_GLOB LIST_DIRECTORIES true "*")

message(STATUS "Searching for build config in ${V_GLOB}")
foreach(item ${V_GLOB})
  if(IS_DIRECTORY ${item})
    FILE(GLOB IS_CMAKE "${item}/CMakeLists.txt")
    if (IS_CMAKE)
      add_subdirectory(${item})
      message ("Adding library: ${item}")
    endif()
  endif()
endforeach()