cmake_minimum_required(VERSION 2.8)

project(pretty_print)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    include/pretty_print/pretty_print.hpp
    include/pretty_print/internal/detail_pretty_print.hpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
