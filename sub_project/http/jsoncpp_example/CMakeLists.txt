cmake_minimum_required(VERSION 3.5)

project(jsoncpp_example)

find_package(jsoncpp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} jsoncpp_lib)