cmake_minimum_required(VERSION 3.22)
project(proj04src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(proj04src
        bet_driver.cpp
        line_parser.cpp
        opnum.h
        token.h bet.h bet.hpp)
