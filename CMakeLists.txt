cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

set (CMAKE_CXX_STANDARD c++17)
project ("AxonEngine" VERSION 0.0.1)

include (${CMAKE_HOME_DIRECTORY}/cmake/build-utils.cmake)

if(GTK2_${_var}_FOUND)
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} ${GTK2_${_var}_LIBRARY})
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} PARENT_SCOPE)
endif()

enable_testing()

add_subdirectory(libraries)

# Examples
set (BUILD_EXAMPLES 1)
add_subdirectory(examples)

# Unit tests
add_subdirectory(tests)
