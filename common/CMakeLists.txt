if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AxonEngine.common")


set(EVENT_SYSTEM
	${CMAKE_CURRENT_SOURCE_DIR}/events/AxonEvent.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/events/AxonEvent.cpp
)

set (SERIALIZATION
	${CMAKE_CURRENT_SOURCE_DIR}/serialization/serialization.h
	${CMAKE_CURRENT_SOURCE_DIR}/serialization/serialization.c
	${CMAKE_CURRENT_SOURCE_DIR}/serialization/serialization.hpp
)

set (BACKEND_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/backends/berkeley/basic_networking.h
	${CMAKE_CURRENT_SOURCE_DIR}/backends/berkeley/basic_networking.c
	${CMAKE_CURRENT_SOURCE_DIR}/backends/berkeley/master_include.h
	${CMAKE_CURRENT_SOURCE_DIR}/backends/backend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backends/backend.h
)

set (NETWORKING
	${EVENT_SYSTEM}
	${BACKEND_SOURCES}
	${SERIALIZATION}
	${BACKEND_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/networking/AxonNetwork.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/networking/AxonNetwork.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/networking/message/AxonMessage.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/networking/message/AxonMessage.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(NetworkingLib SHARED ${NETWORKING})

target_include_directories(NetworkingLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(NetworkingLib PUBLIC AXON_LIB)

set (EVENT_SYSTEM ${EVENT_SYSTEM} PARENT_SCOPE)
set (SERIALIZATION ${SERIALIZATION} PARENT_SCOPE)
set (BACKEND_SOURCES ${BACKEND_SOURCES} PARENT_SCOPE)
set (NETWORKING ${NETWORKING} PARENT_SCOPE)

if(WIN32)
  target_link_libraries(NetworkingLib ws2_32)
endif(WIN32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NetworkingLib PROPERTY CXX_STANDARD 17)
endif()

