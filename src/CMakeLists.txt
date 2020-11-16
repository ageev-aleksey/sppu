set(SRC_MODEL ${CMAKE_CURRENT_LIST_DIR}/model/SddModel.cpp
              ${CMAKE_CURRENT_LIST_DIR}/model/PwdInputGenerator.cpp)
file(GLOB_RECURSE SRC_GUI ${CMAKE_CURRENT_LIST_DIR}/gui/*.cpp)
file(GLOB_RECURSE SRC_MODEL_COMPONENTS ${CMAKE_CURRENT_LIST_DIR}/model/Q*.cpp)

file(GLOB_RECURSE MATLAB_SIMULINK_SRC ${LIB_DIR}/simulink/*.c
                             ${LIB_DIR}/simulink/*.cpp)
HEADER_DIRECTORIES(MATLAB_SIMULINK_INCLUDE_DIRS ${LIB_DIR}/simulink/)

file(GLOB_RECURSE QT_MOC_HEADERS ${INCLUDE_DIR}/Q*.h)


add_library(sdd_model STATIC ${SRC_MODEL} ${MATLAB_SIMULINK_SRC})
target_include_directories(sdd_model PRIVATE ${INCLUDE_DIR} ${MATLAB_SIMULINK_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QRC ${CMAKE_CURRENT_LIST_DIR}/resource/img.qrc)

QT5_ADD_RESOURCES(${QRC}, QT_RESOURCES)

add_library(qt_model_components STATIC ${SRC_MODEL_COMPONENTS})
target_include_directories(qt_model_components PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS})
target_compile_options(qt_model_components PRIVATE -fPIC)
target_link_libraries(qt_model_components)
add_library(SDD::QT_MODEL_COMPONENTS ALIAS qt_model_components)

add_executable(app_gui ${SRC_GUI} ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${QT_ADDITIONAL_QCUSTOMPLOT}/qcustomplot.cpp  ${QT_MOC_HEADERS} ${QRC})
target_include_directories(app_gui PUBLIC ${INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS}
                            ${Qt5Widgets_INCLUDE_DIRS} ${QT_ADDITIONAL_QCUSTOMPLOT})

target_link_libraries(app_gui sdd_model qt_model_components Qt5::Core Qt5::Widgets Qt5::PrintSupport  pthread)
target_enable_cpplint(app_gui
                      SOURCES ${SRC_GUI}
                      FILTERS "-legal/copyright"
                      LINE_LENGTH 120)
