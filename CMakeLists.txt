cmake_minimum_required(VERSION 3.28)
project(lrite)

set(CMAKE_CXX_STANDARD 23)

add_executable(lrite src/main.cpp
        src/generation.hpp
        src/parser.hpp
        src/generation.hpp)
