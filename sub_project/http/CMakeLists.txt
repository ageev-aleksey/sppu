cmake_minimum_required(VERSION 3.5)

project(http_client)


find_package(CURL REQUIRED)
#find_package(PkgConfig REQUIRED)
find_package(jsoncpp REQUIRED)
#pkg_check_modules(JSONCPP jsoncpp)
#find_package(Qt5 COMPONENTS Core REQUIRED)

#add_executable(${PROJECT_NAME} http_lib/http_client.cpp)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} Qt5::Core)

#add_subdirectory(http_lib)

#set(Matlab_ROOT_DIR /home/nrx/app/matlab2020b2$)
#find_package(Matlab)
#matlab_add_mex(NAME ${PROJECT_NAME}_mex SRC neurla_http_client_mex.cpp)
#target_include_directories(${PROJECT_NAME}_mex PUBLIC ${CURL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME}_mex ${CURL_LIBRARIES} Qt5::Core)

add_library(mex ./mex/neurla_http_client_mex.cpp)
target_link_libraries(mex ${CURL_LIBRARIES} jsoncpp_lib)
target_include_directories(mex PUBLIC ${CURL_INCLUDE_DIR})

add_subdirectory(jsoncpp_example)