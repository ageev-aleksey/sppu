set(SRC_MODEL ${CMAKE_CURRENT_LIST_DIR}/model/SddModel.cpp
              ${CMAKE_CURRENT_LIST_DIR}/model/PwdInputGenerator.cpp)


file(GLOB_RECURSE SRC_GUI ${CMAKE_CURRENT_LIST_DIR}/gui/*.cpp)
file(GLOB_RECURSE SRC_ALGORITHMS ${CMAKE_CURRENT_LIST_DIR}/algorithms/*.cpp)
file(GLOB_RECURSE SRC_MODEL_COMPONENTS ${CMAKE_CURRENT_LIST_DIR}/model/Q*.cpp)
file(GLOB_RECURSE QT_MOC_SDD_MODEL_HEADERS ${INCLUDE_DIR}/model/Q*.h)

file(GLOB_RECURSE MATLAB_SIMULINK_SRC ${LIB_DIR}/simulink/*.c
                             ${LIB_DIR}/simulink/*.cpp)
HEADER_DIRECTORIES(MATLAB_SIMULINK_INCLUDE_DIRS ${LIB_DIR}/simulink/)


file(GLOB_RECURSE QT_MOC_GUI ${INCLUDE_DIR}/gui/Q*.h)
file(GLOB_RECURSE QT_MOC_MODEL ${INCLUDE_DIR}/model/Q*.h)
file(GLOB_RECURSE QT_MOC_SDD_PROTO ${INCLUDE_DIR}/sdd_protocol/Q*.h)
file(GLOB_RECURSE QT_MOC_SERIALIZATION ${INCLUDE_DIR}/serialization/Q*.h)
file(GLOB_RECURSE QT_MOC_CAMERA ${INCLUDE_DIR}/camera/Q*.h)


file(GLOB_RECURSE SRC_SERIALIZATION ${CMAKE_CURRENT_LIST_DIR}/serialization/*.cpp)
file(GLOB_RECURSE SRC_SDD_PROTOCOL ${CMAKE_CURRENT_LIST_DIR}/sdd_protocol/*.cpp)
file(GLOB_RECURSE QT_MOC_SDD_PROTOCOL_HEADERS ${INCLUDE_DIR}/sdd_protocol/Q*.h)

file(GLOB_RECURSE SRC_CAMERA ${CMAKE_CURRENT_LIST_DIR}/camera/*.cpp)
file(GLOB_RECURSE QT_MOC_CAMERA_HEADERS ${INCLUDE_DIR}/camera/Q*.h)

add_library(sdd_model STATIC ${SRC_MODEL} ${MATLAB_SIMULINK_SRC})
target_include_directories(sdd_model PRIVATE ${INCLUDE_DIR} ${MATLAB_SIMULINK_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS SerialPort REQUIRED)
find_package(Boost REQUIRED COMPONENTS container)
find_package(OpenCV REQUIRED)
find_package(libjpeg-turbo REQUIRED)

if(Boost_FOUND) 
    message(STATUS "BOOST: ${Boost_LIBRARY_DIRS} : ${Boost_LIBRARIES}")
    message(STATUS "BOOST headers: ${Boost_INCLUDE_DIRS}")
endif()

if(OpenCV_FOUND)
    message("======[ OpenCV FOUND ]======")
    message("Version: " ${OpenCV_VERSION})
    message("libs: " ${OpenCV_LIBS})
    message("include dirs: " ${OpenCV_INCLUDE_DIRS})
    message("============================")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QRC ${RESOURCE_DIR}/img.qrc)

QT5_ADD_RESOURCES(${QRC}, QT_RESOURCES)

add_library(qt_model_components STATIC ${SRC_MODEL_COMPONENTS})
target_include_directories(qt_model_components PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS})
target_compile_options(qt_model_components PRIVATE -fPIC)
target_link_libraries(qt_model_components sdd_protocol)
add_library(SDD::QT_MODEL_COMPONENTS ALIAS qt_model_components)


add_library(camera ${SRC_CAMERA} ${QT_MOC_CAMERA})
target_include_directories(camera PUBLIC ${INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ${CMAKE_INCLUDE_CURRENT_DIR})
target_link_libraries(camera PUBLIC ${OpenCV_LIBS} Qt5::Core Qt5::Widgets libjpeg-turbo::jpeg algorithms)

add_executable(app_gui ${SRC_GUI} ${CMAKE_CURRENT_LIST_DIR}/main_SddClient.cpp
        ${LIB_DIR}/qcustomplot/qcustomplot.cpp
        ${QT_MOC_GUI}
        ${QT_MOC_MODEL}
        ${QT_MOC_SDD_PROTO}
        ${QT_MOC_SERIALIZATION}
        ${QT_MOC_CAMERA}
        ${QRC})
target_include_directories(app_gui PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS}
                            ${Qt5Widgets_INCLUDE_DIRS} ${LIB_DIR}/qcustomplot/
                             ${Boost_INCLUDE_DIRS})

target_link_libraries(app_gui sdd_model qt_model_components camera Qt5::Core Qt5::Widgets Qt5::PrintSupport ${Boost_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(app_gui pthread atomic)
endif()
#target_enable_cpplint(app_gui
#                      SOURCES ${SRC_GUI}
#                      FILTERS "-legal/copyright"
#                      LINE_LENGTH 120)




add_library(sdd_protocol ${SRC_SDD_PROTOCOL} ${SRC_SERIALIZATION} ${QT_MOC_SDD_PROTOCOL_HEADERS})
target_include_directories(sdd_protocol PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5SerialPort_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})
target_link_libraries(sdd_protocol  ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES})

add_executable(model_to_com_port ${QT_MOC_SDD_MODEL_HEADERS} ${CMAKE_CURRENT_LIST_DIR}/main_ModelToComPort.cpp)
target_include_directories(model_to_com_port PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS}
                                                    ${Qt5SerialPort_INCLUDE_DIRS})
target_link_libraries(model_to_com_port ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES}
                                        qt_model_components sdd_model sdd_protocol)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(model_to_com_port pthread atomic)
endif()

add_library(algorithms ${SRC_ALGORITHMS})
target_include_directories(algorithms PUBLIC ${INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(algorithms ${OpenCV_LIBS})

add_executable(camera_app main_camera.cpp)
target_include_directories(camera_app PUBLIC ${INCLUDE_DIR})
target_link_libraries(camera_app camera Qt5::Core Qt5::Widgets)


add_custom_command(
        TARGET model_to_com_port PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCE_DIR}/model_config.json
        ${CMAKE_CURRENT_BINARY_DIR}/model_config.json)


install(TARGETS app_gui model_to_com_port
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/bin
        )