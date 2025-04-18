project("AxonEngine.libraries")

file(GLOB V_GLOB LIST_DIRECTORIES true "*")
message(STATUS "Searching for build config in ${V_GLOB}")

set (COMMON_SRC "AxonUtility.h" PARENT_SCOPE)

foreach(item ${V_GLOB})
    if(IS_DIRECTORY ${item})
        FILE(GLOB IS_CMAKE "${item}/CMakeLists.txt")
        if (IS_CMAKE)
            add_subdirectory(${item})
            message ("Adding library: ${item}")
        endif()
    endif()
endforeach()
