cmake_minimum_required(VERSION 3.11)
set(TARGET "firstsecondthird")
project(${TARGET})

set(CMAKE_C_FLAGS "-Wall -pthread")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++20 -lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

include_directories(
    /usr/include
    /usr/local/include
    /usr/include/glib-2.0
    /usr/include/gstreamer-1.0
)

link_directories(
    /usr/local/lib
)

set(SOURCE_FILES
        ${TARGET}.cpp
        )

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(DYNAMIC_LIBRARIES
            libm.so
            libz.so
            librt.so
            libdl.so
        )
    set(STATIC_LIBRARIES
        )
endif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)

add_executable(${TARGET} ${SOURCE_FILES})

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

target_link_libraries(${TARGET} ${DYNAMIC_LIBRARIES} ${STATIC_LIBRARIES} )
