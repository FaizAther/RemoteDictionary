//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_TCP_SERVER_H
#define TCP_SOCKET_TCP_SERVER_H

#include <sys/poll.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define TCP_POLL_CHUNK_SZ 1024

#define TCP_DEFAULT_BACKLOG 8

#define TCP_DEFAULT_IP "127.0.0.1"

#define TCP_DEFAULT_PORT 5000

#define TCP_BUF_SZ 1460

typedef struct tcp_str_t {
    char buf[TCP_BUF_SZ];
    int buf_sz;
    struct tcp_str_t *next;
} tcp_str_t;
typedef tcp_str_t *tcp_str;

typedef struct tcp_client_t {
    int fd;
    tcp_str buf;
    struct sockaddr_in addr;
    socklen_t addr_sz;
    struct tcp_client_t *next;
    struct tcp_client_t *prev;
} tcp_client_t;
typedef tcp_client_t *tcp_client;

typedef struct tcp_clients_list_t {
    tcp_client head;
    tcp_client tail;
    uint32_t size;
} tcp_clients_list_t;
typedef tcp_clients_list_t *tcp_clients_list;

typedef struct tcp_socket_t {
    int fd;
    struct pollfd pfds[TCP_POLL_CHUNK_SZ];
    tcp_clients_list_t clients;
    struct sockaddr_in addr;
    socklen_t addr_sz;
    int err;
} tcp_socket_t;
typedef tcp_socket_t *tcp_socket;

tcp_socket_t tcpSocket; /* Global Var */

/*
 *
 * */
int
socket_init();

/*
 *
 * */
int
socket_bind();

/*
 *
 * */
int
socket_listen();

/*
 *
 * */
int
socket_accept();

/*
 *
 * */
int
socket_recv();

/*
 *
 * */
int
socket_send();

/*
 *
 * */
int
server_start();

#endif //TCP_SOCKET_TCP_SERVER_H
