cmake_minimum_required(VERSION 2.8)

project(pretty_print)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

install(FILES include/pretty_print/pretty_print.hpp DESTINATION include/pretty_print)
install(FILES include/pretty_print/internal/detail_pretty_print.hpp DESTINATION include/pretty_print/internal)
