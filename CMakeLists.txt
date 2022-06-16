cmake_minimum_required (VERSION 3.10)
project (tcp_socket)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(tcp_socket main.cpp tcp_server.c tcp_server.h Dictionary.cpp Dictionary.h)
