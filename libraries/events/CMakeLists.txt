project ("AxonEngine.libraries.events")

file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.cpp" "*.hpp" "*.ipp")
add_library(events STATIC ${SOURCES})

target_include_directories(events PUBLIC "${LIBS_DIR}")
