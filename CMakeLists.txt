cmake_minimum_required(VERSION 3.5.0)
project(NP VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions(UNICODE)
add_executable(NP main.cpp)

