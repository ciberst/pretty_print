cmake_minimum_required(VERSION 2.8)

project(pretty_print)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

install()
