project("AxonEngine.libraries")

file(GLOB_RECURSE V_GLOB LIST_DIRECTORIES true "*")
message(STATUS "Searching for build config in ${V_GLOB}")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set (COMMON_SRC "AxonUtility.h" PARENT_SCOPE)
set (LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_all_subdirs()