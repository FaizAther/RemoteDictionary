//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_TCP_SERVER_H
#define TCP_SOCKET_TCP_SERVER_H

#include <sys/poll.h>
#include <arpa/inet.h>

#define TCP_POLL_CHUNK_SZ 1024

#define TCP_DEFAULT_BACKLOG 8

#define TCP_DEFAULT_IP "127.0.0.1"

#define TCP_DEFAULT_PORT 5000

#define TCP_BUF_SZ 1460

#define CHECK_USAGE 5

typedef struct tcp_str_t {
    char sbuf[TCP_BUF_SZ];
    size_t sbuf_sz;
    char *buf;
    ssize_t buf_sz;
    struct tcp_str_t *next;
} tcp_str_t;
typedef tcp_str_t *tcp_str;

typedef struct tcp_client_t {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_sz;
    tcp_str_t reader_str;
    tcp_str_t writer_str;
    pthread_t reader_thread;
    pthread_t writer_thread;
    pthread_mutex_t reader_stick;
    pthread_mutex_t writer_stick;
    struct tcp_client_t *next;
    struct tcp_client_t *prev;
} tcp_client_t;
typedef tcp_client_t *tcp_client;

typedef struct tcp_clients_list_t {
    tcp_client head;
    tcp_client tail;
    tcp_client curr;
    uint32_t size;
} tcp_clients_list_t;
typedef tcp_clients_list_t *tcp_clients_list;

typedef struct tcp_socket_t {
    int fd;
    struct pollfd *poll_fds;
    uint32_t poll_fds_sz;
    nfds_t poll_max;
    int poll_ret;
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
void *
socket_recv(void *);

/*
 *
 * */
void *
socket_send(void *);

/*
 *
 * */
_Noreturn int
server_start();

#endif //TCP_SOCKET_TCP_SERVER_H
