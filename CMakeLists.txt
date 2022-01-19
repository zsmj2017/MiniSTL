CMAKE_MINIMUM_REQUIRED(VERSION 3.20.0)
PROJECT(miniSTL)

message("current build type is ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++11 support has been enabled by default.")

add_subdirectory(Source)

if (ENABLE_MINISTL_TEST)
    find_package(GTest REQUIRED)
    add_subdirectory("Test")
endif()