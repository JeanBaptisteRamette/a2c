cmake_minimum_required(VERSION 3.22)
project(a2c)

set(CMAKE_CXX_STANDARD 20)

add_library(a2c SHARED a2c.cpp console.cpp console.hpp structs.hpp memory.hpp utils.hpp detour.cpp detour.hpp symbol.hpp)
