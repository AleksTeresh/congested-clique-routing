cmake_minimum_required(VERSION 3.14)
project(cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cpp
        clique-router.cpp
        clique-router.h
        color.cpp
        color.h
        main.cpp
        node.cpp
        node.h
        test.cpp
        test.h
        time-point.h
        util.h)
