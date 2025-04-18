project ("AxonEngine.libraries")

file(GLOB SOURCES "*.c" "*.h" "*.cpp" "*.hpp")
add_library(events STATIC ${SOURCES})

target_include_directories(events PUBLIC ".")
