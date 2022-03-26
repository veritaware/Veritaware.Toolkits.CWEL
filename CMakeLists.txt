cmake_minimum_required(VERSION 3.14)
project(CWEL)

set(CMAKE_CXX_STANDARD 11)

add_library(CWEL
        src/Veritaware.Toolkits.CWEL.cpp
        src/IGetHashCode.cpp
        src/IToString.cpp
        src/Object.cpp
        src/Color.cpp
        src/VectorExtensions.cpp
        src/Event.cpp
        src/DependencyProperty.cpp
        )

target_include_directories(CWEL PUBLIC include)

target_compile_features(CWEL
        PUBLIC cxx_auto_type
        PUBLIC cxx_defaulted_functions
        )
