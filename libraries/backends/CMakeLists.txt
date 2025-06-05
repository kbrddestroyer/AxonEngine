project ("AxonEngine.libraries.backends")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(backends STATIC ${SOURCES})

target_include_directories(backends PUBLIC ${LIBS_DIR})
target_include_directories(backends PUBLIC "${CMAKE_HOME_DIRECTORY}/common")
target_compile_definitions(backends PUBLIC AXON_LIB)
