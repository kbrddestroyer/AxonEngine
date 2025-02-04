cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine Basic GUI Example")

get_filename_component(ROOT "../../" ABSOLUTE)
set(ROOT ${ROOT}/)

if(WIN32)
	SET(AXON_LIBS_PATH "${ROOT}out/build/x64-debug/")	# Axon output directory for Windows x64
	SET(AXON_CLIENT_LIB "AxonClientLib")
	SET(AXON_SERVER_LIB "AxonServerLib")
	set(AXON_DYNAMIC ".dll")
	set(AXON_STATIC ".lib")
else()
	SET(AXON_LIBS_PATH "${ROOT}cmake-build-debug/")		# Axon output directory for Ubuntu
	SET(AXON_CLIENT_LIB "libAxonClientLib")
	SET(AXON_SERVER_LIB "libAxonServerLib")
	set(AXON_DYNAMIC ".so")
	set(AXON_STATIC ".a")
endif()

link_directories(${AXON_LIBS_PATH})

include_directories("${ROOT}Include")
include_directories("${ROOT}AxonLibrary")
include_directories("${ROOT}AxonLibrary/server")
include_directories("${ROOT}AxonLibrary/client")
include_directories("${ROOT}AxonLibrary/common")

set(MASTER_INCLUDE 
    "${ROOT}Include/Axon.h"
    "${ROOT}Include/AxonEngine.h" "${ROOT}Include/AxonEngine.cpp"
    "${ROOT}Include/AxonEntity.h" "${ROOT}Include/AxonEntity.cpp"
    "${ROOT}Include/AxonError.h" "${ROOT}Include/AxonError.cpp" 
    "${ROOT}Include/AxonTypes.h" "${ROOT}Include/AxonTypes.cpp"
)

add_executable(client "client.cpp" ${MASTER_INCLUDE})
add_executable(server "server.cpp" ${MASTER_INCLUDE})

target_compile_definitions(server PUBLIC AXON_SERVER)
target_compile_definitions(client PUBLIC AXON_CLIENT)

if(WIN32)
  target_link_libraries(client ws2_32)
  target_link_libraries(server ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET client PROPERTY CXX_STANDARD 20)
  set_property(TARGET server PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(client ${AXON_CLIENT_LIB})
target_link_libraries(server ${AXON_SERVER_LIB})

add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${AXON_LIBS_PATH}${AXON_CLIENT_LIB}${AXON_DYNAMIC} ${CMAKE_BUILD_RPATH}$<CONFIG>/${AXON_CLIENT_LIB}${AXON_DYNAMIC})
add_custom_command(TARGET server POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${AXON_LIBS_PATH}${AXON_SERVER_LIB}${AXON_DYNAMIC} ${CMAKE_BUILD_RPATH}$<CONFIG>/${AXON_SERVER_LIB}${AXON_DYNAMIC})
