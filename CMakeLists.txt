cmake_minimum_required(VERSION 3.16)
project(SddClient)

#FOR IDEA
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS ${CMAKE_ROOT})
message(STATUS "COMPILER ID: ${CMAKE_CXX_COMPILER_ID}")
option(COMPILE_TESTS "" OFF)



if(UNIX)
    message(STATUS "UNIX")
    set(OpenCV_DIR ~/Загрузки/opencv/opencv/build)
    #set(Qt5_DIR /home/nrx/Qt/5.15.0/gcc_64/lib/cmake/Qt5)  
    set(Qt5_DIR ~/Qt/5.15.2/gcc_64/lib/cmake/Qt5)
    set(libjpeg-turbo_DIR /opt/libjpeg-turbo/lib64/cmake/libjpeg-turbo)
    #set(Qt5_DIR C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5)
    #Boost
    #set(BOOST_ROOT C:/libs/boost64-1.61.0-vs2015)
    #set(BOOST_INCLUDEDIR "include/boost-1_61")
    #set(BOOST_LIBRARYDIR "lib")
endif()

if (WIN32)
    message(STATUS "WIN32")
    set(libjpeg-turbo_DIR C:/libs/libjpeg-turbo64/lib/cmake/libjpeg-turbo)
    set(OpenCV_DIR C:/libs/opencv/build)
    set(Qt5_DIR C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5)
    #Boost
    set(BOOST_ROOT C:/libs/boost64-1.61.0-vs2015)
    set(BOOST_INCLUDEDIR "include/boost-1_61")
    set(BOOST_LIBRARYDIR "lib")
endif()


####WIDOWS######


################


#include(${CMAKE_CURRENT_LIST_DIR}/MatlabLibrary.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_headers.cmake)

set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(RESOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/resource)

#include(./cmake/Cpplint.cmake)

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
add_subdirectory(sub_project/find_in_image)
add_subdirectory(sub_project/http)
