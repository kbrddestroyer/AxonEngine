project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(networking SHARED ${SOURCES})
set (LIB_HOME ${CMAKE_HOME_DIRECTORY}/libraries)

target_compile_definitions(networking PUBLIC AXON_LIB)
target_link_libraries(networking backends)
target_link_libraries(networking events)
target_link_libraries(networking serialization)
target_include_directories(networking PUBLIC "${LIB_HOME}")
