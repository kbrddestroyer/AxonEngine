project ("AxonEngine.common")

set(EVENT_SYSTEM
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.hpp
	${CMAKE_HOME_DIRECTORY}/common/events/AxonEvent.cpp
)

set (SERIALIZATION
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.h
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.c
	${CMAKE_HOME_DIRECTORY}/common/serialization/serialization.hpp
)

set (NETWORKING
	${EVENT_SYSTEM}
	${BACKEND_SOURCES}
	${SERIALIZATION}
	${CMAKE_HOME_DIRECTORY}/common/networking/AxonNetwork.hpp
	${CMAKE_HOME_DIRECTORY}/common/networking/AxonNetwork.cpp
	${CMAKE_HOME_DIRECTORY}/common/networking/message/AxonMessage.hpp
	${CMAKE_HOME_DIRECTORY}/common/networking/message/AxonMessage.cpp
)

set (EVENT_SYSTEM ${EVENT_SYSTEM} PARENT_SCOPE)
set (SERIALIZATION ${SERIALIZATION} PARENT_SCOPE)
set (NETWORKING ${NETWORKING} PARENT_SCOPE)

add_library(NetworkingLib ${NETWORKING})

target_include_directories(NetworkingLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
