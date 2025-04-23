cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

include (${CMAKE_HOME_DIRECTORY}/cmake/build-utils.cmake)

set (CONAN_TOOLCHAIN "${CMAKE_HOME_DIRECTORY}/conan/build/${CMAKE_BUILD_TYPE}/conan_toolchain.cmake")
include(${CONAN_TOOLCHAIN})

if(GTK2_${_var}_FOUND)
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} ${GTK2_${_var}_LIBRARY})
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} PARENT_SCOPE)
endif()

project ("AxonEngine")
enable_testing()
# Common backends
add_subdirectory(libraries)

# Examples
set (BUILD_EXAMPLES 1)
add_subdirectory(examples)

# Unit tests
add_subdirectory(tests)
