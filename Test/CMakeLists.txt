cmake_minimum_required(VERSION 3.20.0)

project(Test)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE TEST_SRC *.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Source)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} GTest::gtest GTest::gtest_main STL)