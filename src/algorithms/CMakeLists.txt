 cmake_minimum_required(VERSION 3.17)

project(AlgTest)

set(OpenCV_DIR ~/Загрузки/opencv/opencv/build)
find_package(OpenCV REQUIRED)


add_compile_definitions(ALGORITHM_TESTING)
add_executable(${PROJECT_NAME} algorithms.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})