project(ip_web_cam)

#set(OpenCV_DIR /opt/opencv/release)
set(OpenCV_DIR C:/libs/opencv/build)
set(INCLUDE_DIR include)
set(SRC_DIR src)

find_package(Qt5 COMPONENTS Core Widgets Multimedia REQUIRED)
find_package(OpenCV REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp ${SRC_DIR}/Shot.cpp ${INCLUDE_DIR}/Shot.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${OpenCV_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}  Qt5::Core Qt5::Widgets Qt5::Multimedia ${OpenCV_LIBS})
