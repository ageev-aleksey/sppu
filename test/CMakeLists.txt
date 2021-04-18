file(GLOB TEST_SRC ${CMAKE_CURRENT_LIST_DIR}/*.h
                    ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

find_package(GTest REQUIRED)

add_executable(unit_test ${TEST_SRC})
target_include_directories(unit_test PUBLIC ${TEST_INCLUDE_DIR}  ${INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})
target_link_libraries(unit_test sdd_protocol ${GTEST_BOTH_LIBRARIES} pthread)