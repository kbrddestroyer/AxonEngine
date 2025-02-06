cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.examples.basics.basic-init")

add_executable(TestFile "main.cpp" ${BACKEND_SOURCES})

if(WIN32)
  target_link_libraries(TestFile ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestFile PROPERTY CXX_STANDARD 17)
endif()
