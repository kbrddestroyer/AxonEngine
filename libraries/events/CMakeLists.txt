project ("AxonEngine.libraries")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(events STATIC ${SOURCES})

target_include_directories(events PUBLIC ".")
