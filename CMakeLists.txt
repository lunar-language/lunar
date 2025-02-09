cmake_minimum_required(VERSION 3.28)
project(lrite)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

add_executable(lrite src/main.cpp
        src/generation.hpp
        src/parser.hpp
        src/generation.hpp)
