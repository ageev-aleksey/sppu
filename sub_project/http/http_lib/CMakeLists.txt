project(http_client_lib)

find_package(CURL REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(${PROJECT_NAME} STATIC http.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Qt5::Core ${CURL_LIBRARIES})