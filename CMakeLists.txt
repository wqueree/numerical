cmake_minimum_required(VERSION 3.5.0)
project(numerical-algorithms VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(numerical-algorithms main.cpp)

