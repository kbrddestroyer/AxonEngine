include_directories(${CMAKE_HOME_DIRECTORY}/libraries)

include ("CTest")

file(GLOB_RECURSE V_GLOB LIST_DIRECTORIES true "*")

message(STATUS "Searching for build config in ${V_GLOB}")

add_all_subdirs()
