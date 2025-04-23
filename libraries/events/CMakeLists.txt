project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(events STATIC ${SOURCES})

target_compile_definitions(events PUBLIC AXON_LIB)

set (LIB_HOME ${CMAKE_HOME_DIRECTORY}/libraries)
target_include_directories(events PUBLIC "${LIB_HOME}")
