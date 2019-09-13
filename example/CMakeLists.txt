cmake_minimum_required(VERSION 2.8)

project(example)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/../include"
)
