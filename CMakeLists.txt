cmake_minimum_required(VERSION 3.16)
project(CWEL VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cwel INTERFACE)
target_include_directories(cwel INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(CWEL_BUILD_TESTS "Build unit tests" ON)

if(CWEL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
