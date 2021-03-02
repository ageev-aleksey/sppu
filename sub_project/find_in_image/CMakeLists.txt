cmake_minimum_required(VERSION 3.5)
project(openCvTest)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR /opt/opencv/release)
find_package(OpenCV REQUIRED)

add_executable(openCvTest main.cpp)
target_include_directories(openCvTest PUBLIC  ${OpenCV_INCLUDE_DIRS})
target_link_libraries(openCvTest ${OpenCV_LIBS})