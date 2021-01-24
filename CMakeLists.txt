cmake_minimum_required(VERSION 3.16)
project(SddClient)

#FOR IDEA
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS ${CMAKE_ROOT})
option(COMPILE_TESTS "" ON)


set(Qt5_DIR /home/nrx/Qt/5.15.0/gcc_64/lib/cmake/Qt5)
set(QT_ADDITIONAL_QCUSTOMPLOT /home/nrx/Qt/5.15.0/additional/qcustomplot)


#include(${CMAKE_CURRENT_LIST_DIR}/MatlabLibrary.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_headers.cmake)

set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(RESOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/resource)

include(./cmake/Cpplint.cmake)

add_subdirectory(${SRC_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Using debug GCC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    endif()
endif()

if(COMPILE_TESTS)
    message(STATUS "Compiling unit test")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()


add_subdirectory(sub_project/ip_cam)