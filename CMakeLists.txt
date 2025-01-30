cmake_minimum_required(VERSION 3.29)
project(fe_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(fe_parser main.cpp
        app/application.cpp
        app/application.h)
