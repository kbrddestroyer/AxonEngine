project ("AxonEngine.libraries.networking" LANGUAGES C CXX)
SET (CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ipp"
)

if (AXON_STATIC)
    add_library(networking STATIC ${SOURCES})
    add_compile_definitions(AXON_STATIC)
else()
    add_library(networking SHARED ${SOURCES})
endif()

target_compile_definitions(networking PUBLIC AXON_LIB)
target_link_libraries(networking events serialization backends messages)

target_include_directories(networking PUBLIC "${LIBS_DIR}")
target_include_directories(networking PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
