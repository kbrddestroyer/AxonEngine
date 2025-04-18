project ("AxonEngine.libraries")

file(GLOB SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(backends STATIC ${SOURCES})

target_include_directories(backends PUBLIC ".")
