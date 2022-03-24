cmake_minimum_required(VERSION 3.14)
project(Veritaware.Toolkits.CWEL)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME}
        src/Veritaware.Toolkits.CWEL.cpp
        src/IGetHashCode.cpp
        src/IToString.cpp
        src/Object.cpp
        src/Color.cpp
        src/VectorExtensions.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC include)

target_compile_features(${PROJECT_NAME}
        PUBLIC cxx_auto_type
        PUBLIC cxx_defaulted_functions)
