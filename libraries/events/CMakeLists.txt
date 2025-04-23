project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp" "*.ipp")
add_library(events STATIC ${SOURCES})

set (LIB_HOME ${CMAKE_HOME_DIRECTORY}/libraries)
target_include_directories(events PUBLIC "${LIB_HOME}")
