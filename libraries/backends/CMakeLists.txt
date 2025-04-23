project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(backends STATIC ${SOURCES})

target_include_directories(backends PUBLIC "${CMAKE_HOME_DIRECTORY}/libraries")
