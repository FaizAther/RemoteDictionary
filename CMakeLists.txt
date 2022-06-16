cmake_minimum_required (VERSION 3.0)
project (tcp_socket)

add_executable(tcp_socket main.cpp tcp_server.c tcp_server.h Dictionary.cpp Dictionary.h)
