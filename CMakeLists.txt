cmake_minimum_required (VERSION 2.8.11)
project (tcp_socket)

add_executable(tcp_socket main.cpp tcp_server.c tcp_server.h)
