file(GLOB TEST_SRC ${CMAKE_CURRENT_LIST_DIR}/*.h
                    ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

find_package(Qt5 COMPONENTS Core Test REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(test ${TEST_SRC})
target_include_directories(test PUBLIC ${TEST_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Test_INCLUDE_DIRS} )
target_link_libraries(test SDD::QT_MODEL_COMPONENTS Qt5::Test Qt5::Core)