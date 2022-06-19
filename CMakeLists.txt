cmake_minimum_required(VERSION 3.22)
project (tcp_socket)

set(CMAKE_CXX_STANDARD 20)

add_executable(tcp_socket mainS.cpp tcp_server.c tcp_server.h Dictionary.cpp Dictionary.h DictionaryConnector.cpp DictionaryConnector.h)

add_executable(tcp_client mainC.cpp tcp_client.c tcp_client.h)
