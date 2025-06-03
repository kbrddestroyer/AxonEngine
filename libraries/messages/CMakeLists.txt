project ("AxonEngine.libraries.messages")

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ipp"
)

add_library(messages STATIC ${SOURCES})

target_compile_definitions(messages PUBLIC AXON_LIB)
target_link_libraries(messages events serialization backends)
target_include_directories(messages PUBLIC "${LIBS_DIR}")
