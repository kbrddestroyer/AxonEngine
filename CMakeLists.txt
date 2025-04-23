cmake_minimum_required (VERSION 3.8)

include (${CMAKE_HOME_DIRECTORY}/cmake/build-utils.cmake)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(GTK2_${_var}_FOUND)
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} ${GTK2_${_var}_LIBRARY})
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} PARENT_SCOPE)
endif()

project ("AxonEngine")
include ("CTest")
# Common backends

add_subdirectory(libraries)

# Examples
set (BUILD_EXAMPLES 1)
add_subdirectory(examples)

# Unit tests
add_subdirectory(tests)
