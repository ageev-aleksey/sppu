
cmake_minimum_required(VERSION 3.5)
project(openCvTest)

set(CMAKE_CXX_STANDARD 17)
#set(OpenCV_DIR ~/Загрузки/opencv/opencv-master/build)
find_package(OpenCV REQUIRED)
find_package(libjpeg-turbo REQUIRED)

add_executable(openCvTest main.cpp)
target_include_directories(openCvTest PUBLIC  ${OpenCV_INCLUDE_DIRS})
target_link_libraries(openCvTest ${OpenCV_LIBS})

add_executable(find_red_circle main_find_red_circle.cpp)
target_include_directories(find_red_circle PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(find_red_circle ${OpenCV_LIBS})

add_executable(read_jpg_from_buffer libjpeg_read_from_buffer.cpp)
target_include_directories(read_jpg_from_buffer PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(read_jpg_from_buffer ${OpenCV_LIBS} libjpeg-turbo::jpeg)

add_executable(libjpeg_example lib_jpeg_example.cpp)
target_link_libraries(read_jpg_from_buffer libjpeg-turbo::jpeg)

