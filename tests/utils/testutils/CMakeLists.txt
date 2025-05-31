file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ipp"
)

add_library(testutils STATIC ${SOURCES})
set (LIB_HOME ${CMAKE_HOME_DIRECTORY}/test/utils/testutils)

target_include_directories(networking PUBLIC "${LIB_HOME}")
