//
// Created by macos on 6/15/22.
//

#include "tcp_server.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 *
 * */
int
socket_init()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    memset(&tcpSocket, 0, sizeof(tcpSocket));
    tcpSocket.addr.sin_len = tcpSocket.addr_sz = sizeof(tcpSocket.addr);

    if (tcpSocket.fd = socket(AF_INET, SOCK_STREAM, 0), tcpSocket.fd < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }
    fprintf(stderr, "ERR: fd = %d\n", tcpSocket.fd);

    if (setsockopt(tcpSocket.fd, SOL_SOCKET, SO_REUSEADDR, &tcpSocket.addr, tcpSocket.addr_sz) == -1) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    int flags = fcntl(tcpSocket.fd, F_GETFL);
    if (flags < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    if (fcntl(tcpSocket.fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    tcpSocket.addr.sin_family = AF_INET;
    tcpSocket.addr.sin_port = htons(TCP_DEFAULT_PORT);
    tcpSocket.addr.sin_addr.s_addr = inet_addr(TCP_DEFAULT_IP);

    memset(&tcpSocket.clients, 0, sizeof(tcp_clients_list_t));

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
socket_bind()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    if (bind(tcpSocket.fd, (struct sockaddr *)&tcpSocket.addr, tcpSocket.addr_sz) < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    fprintf(stdout, "Address={%s}, port={%d}\n", TCP_DEFAULT_IP, ntohs(tcpSocket.addr.sin_port));

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
socket_listen()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    if (listen(tcpSocket.fd, TCP_DEFAULT_BACKLOG) < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
socket_accept()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    struct sockaddr_in t_client;
    memset(&t_client, 0, sizeof(struct sockaddr_in));
    socklen_t t_client_sz = sizeof(struct sockaddr_in);
    int t_client_fd = -1;

    if (t_client_fd = accept(tcpSocket.fd, (struct sockaddr *)&t_client, &t_client_sz), t_client_fd < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
        return -1;
    } else {
        fprintf(stdout, "Address={%s}, port={%d}\n", TCP_DEFAULT_IP, ntohs(t_client.sin_port));
    }

    if (tcpSocket.clients.head == NULL) {
        tcpSocket.clients.head = tcpSocket.clients.tail = (tcp_client)malloc(sizeof(tcp_client_t));
        memset(tcpSocket.clients.head, 0, sizeof(tcp_client_t));
        tcpSocket.clients.size += 1;
    } else {
        tcpSocket.clients.tail->next = (tcp_client)malloc(sizeof(tcp_client_t));
        tcpSocket.clients.tail->next->prev = tcpSocket.clients.tail;
        tcpSocket.clients.tail = tcpSocket.clients.tail->next;
        tcpSocket.clients.size += 1;
    }

    memcpy(&tcpSocket.clients.tail->addr, &t_client, sizeof(struct sockaddr_in));
    tcpSocket.clients.tail->addr_sz = t_client_sz;
    tcpSocket.clients.tail->fd = t_client_fd;

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
socket_recv()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
socket_send()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}

/*
 *
 * */
int
server_start()
{
    fprintf(stderr, "ERR: %s __start\n", __func__ );

    fprintf(stderr, "ERR: %s __end\n", __func__ );
    return 0;
}
