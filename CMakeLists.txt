cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(GTK2_${_var}_FOUND)
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} ${GTK2_${_var}_LIBRARY})
   set(GTK2_LIBRARIES ${GTK2_LIBRARIES} PARENT_SCOPE)
endif()

project ("AxonEngine")
include("CTest")
# Common backends

set (BACKEND_SOURCES
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/basic_networking.h
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/basic_networking.c
	${CMAKE_HOME_DIRECTORY}/backends/berkeley/master_include.h
	${CMAKE_HOME_DIRECTORY}/backends/backend.hpp
	${CMAKE_HOME_DIRECTORY}/backends/backend.h
)

add_subdirectory(common)

set (AXON_MASTER
	${NETWORKING}
)

# Examples
add_subdirectory(examples)

# Unit tests
add_subdirectory(tests)
