cmake_minimum_required(VERSION 3.16)
project(TestSimulinkModel)

set(CMAKE_CXX_STANDARD 20)
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

if(COMPILE_TESTS)
    message(STATUS "Compiling unit test")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()