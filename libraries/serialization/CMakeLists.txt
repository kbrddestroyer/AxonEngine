project ("AxonEngine.libraries")

file(GLOB SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(serialization STATIC ${SOURCES} ${COMMON_SRC})
