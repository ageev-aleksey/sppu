cmake_minimum_required(VERSION 3.5)

project(http_client)


find_package(CURL REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(${PROJECT_NAME} http_client.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} Qt5::Core)