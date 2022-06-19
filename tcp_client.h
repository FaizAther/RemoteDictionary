//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_TCP_CLIENT_H
#define TCP_SOCKET_TCP_CLIENT_H

#include <arpa/inet.h>

#define TCP_DEFAULT_IP "127.0.0.1"

#define TCP_BUF_SZ 1460

typedef struct tcp_client_socket_t {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_sz;
    int err;
} tcp_client_socket_t;
typedef tcp_client_socket_t *tcp_client_socket;

tcp_client_socket_t tcpClient;

int
client_init(uint32_t port);

_Noreturn int
client_handle();

#endif //TCP_SOCKET_TCP_CLIENT_H
