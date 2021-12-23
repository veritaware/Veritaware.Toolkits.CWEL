cmake_minimum_required(VERSION 3.14)
project(Veritaware.Toolkits.CWEL VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME}
    source/Veritaware.Toolkits.CWEL.cpp
    source/hashing.cpp
    source/streaming.cpp
    source/color.cpp
    source/vector_ext.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PUBLIC source
)

target_compile_features(${PROJECT_NAME}
    PUBLIC cxx_auto_type
    PUBLIC cxx_defaulted_functions
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)

include(CPack)
