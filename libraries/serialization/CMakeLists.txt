project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(serialization STATIC ${SOURCES} ${COMMON_SRC})
