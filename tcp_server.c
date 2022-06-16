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
#include <unistd.h>

void
exit_func (int signo)
{
    fprintf(stderr, \
		"\nGot signal# {%d}\nTerminating...\n\n", \
		signo);

    close(tcpSocket.fd);

    fprintf(stderr, "SLEEP: %u seconds, check memusage\n", CHECK_USAGE);

    sleep(CHECK_USAGE);

    //pthread_exit(0);
    exit(EXIT_SUCCESS);
}

/*
 *
 * */
int
socket_init()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

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

    tcpSocket.poll_max = 1;

    tcpSocket.poll_fds = (struct pollfd *)malloc(sizeof(struct pollfd) * TCP_POLL_CHUNK_SZ);
    tcpSocket.poll_fds_sz = TCP_POLL_CHUNK_SZ;

    tcpSocket.poll_fds->fd = tcpSocket.fd;
    tcpSocket.poll_fds->events = POLLIN | POLLPRI | POLLERR;

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
int
socket_bind()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    if (bind(tcpSocket.fd, (struct sockaddr *)&tcpSocket.addr, tcpSocket.addr_sz) < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    fprintf(stdout, "Socket Address={%s}, port={%d}\n", TCP_DEFAULT_IP, ntohs(tcpSocket.addr.sin_port));

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
int
socket_listen()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    if (listen(tcpSocket.fd, TCP_DEFAULT_BACKLOG) < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
    }

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
int
socket_accept()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    struct sockaddr_in t_client;
    memset(&t_client, 0, sizeof(struct sockaddr_in));
    socklen_t t_client_sz = sizeof(struct sockaddr_in);
    int t_client_fd;

    if (t_client_fd = accept(tcpSocket.fd, (struct sockaddr *)&t_client, &t_client_sz), t_client_fd < 0) {
        fprintf(stderr, "ERR: %s\n", strerror(errno));
        return -1;
    } else {
        fprintf(stdout, "Client Address={%s}, port={%d}\n", TCP_DEFAULT_IP, ntohs(t_client.sin_port));
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

    tcpSocket.poll_fds[tcpSocket.poll_max].fd = t_client_fd;
    tcpSocket.poll_fds[tcpSocket.poll_max].events = POLLIN | POLLPRI | POLLERR;

    tcpSocket.poll_max += 1;

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
int
socket_recv()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
int
socket_send()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 *
 * */
_Noreturn int
server_start()
{
    fprintf(stderr, "ERR: %s __start\n", __func__);

    signal(SIGKILL, exit_func);
    signal(SIGTERM, exit_func);
    signal(SIGINT,  exit_func);

    socket_init();
    socket_bind();
    socket_listen();

    tcp_socket sock = &tcpSocket;

    while (1) {
        tcpSocket.poll_ret = poll(tcpSocket.poll_fds, tcpSocket.poll_max, -1);
        fprintf(stderr, "ERR: %s %d\n", "poll returned", tcpSocket.poll_ret);

        if (tcpSocket.poll_fds[0].revents & POLLIN) { /* New Connection */
            socket_accept();
            tcpSocket.poll_ret -= 1;
        }

        for (uint32_t i = 1; i <= tcpSocket.poll_max && tcpSocket.poll_ret > 0; i++) {
            if (tcpSocket.poll_fds[i].revents & POLLHUP) { /* Close Connection */
                tcpSocket.poll_fds[i].fd = -1;
                tcpSocket.poll_fds[i].events = 0;
                tcpSocket.poll_fds[i].revents = 0;
                tcpSocket.poll_ret -= 1;
            }
        }
    }
    /* Unreachable */
}
