cmake_minimum_required(VERSION 3.20)
project(CWEL)

set(CMAKE_CXX_STANDARD 20)

add_library(CWEL
        src/Veritaware.Toolkits.CWEL.cpp
        src/Event.cpp
        src/IComparable.cpp
        src/IGetHashCode.cpp
        src/IToString.cpp
        src/Object.cpp
        src/Property.cpp
        src/Extensions/Vector.cpp
        src/System/Command.cpp
)

target_include_directories(CWEL PUBLIC include)

target_compile_features(CWEL
        PUBLIC cxx_auto_type
        PUBLIC cxx_defaulted_functions
        )
