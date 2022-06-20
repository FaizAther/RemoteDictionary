//
// Created by macos on 6/15/22.
//

#include "tcp_server.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

void
exit_func (int signo) {
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
socket_init() {
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
socket_bind() {
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
socket_listen() {
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
socket_accept() {
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
        tcpSocket.clients.size += 1;
    } else {
        tcpSocket.clients.tail->next = (tcp_client)malloc(sizeof(tcp_client_t));
        tcpSocket.clients.tail->next->prev = tcpSocket.clients.tail;
        tcpSocket.clients.tail = tcpSocket.clients.tail->next;
        tcpSocket.clients.size += 1;
    }
    memset(tcpSocket.clients.tail, 0, sizeof(tcp_client_t));

    memcpy(&tcpSocket.clients.tail->addr, &t_client, sizeof(struct sockaddr_in));
    tcpSocket.clients.tail->addr_sz = t_client_sz;
    tcpSocket.clients.tail->fd = t_client_fd;
    pthread_mutex_init(&tcpSocket.clients.tail->reader_stick, NULL);
    pthread_mutex_init(&tcpSocket.clients.tail->writer_stick, NULL);

    tcpSocket.poll_fds[tcpSocket.poll_max].fd = t_client_fd;
    tcpSocket.poll_fds[tcpSocket.poll_max].events = POLLIN | POLLPRI | POLLERR;

    tcpSocket.poll_max += 1;

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return 0;
}

/*
 * A simple memcpy for now
 * TODO: Some JSON library
 * */
size_t
sock_process(tcp_client client) {
    Dictionary_process(client->reader_str.sbuf);
    memcpy(client->writer_str.sbuf, client->reader_str.sbuf, TCP_BUF_SZ);
    client->writer_str.sbuf_sz = client->reader_str.sbuf_sz;
    return client->reader_str.sbuf_sz;
}

/*
 *
 * */
void *
socket_recv(void *vclient) {
    tcp_client client = (tcp_client)vclient;
    int fd = client->fd;
    fprintf(stderr, "ERR: %s __start\n", __func__);
    int err, sz = 0;
    ssize_t read_sz = 0;
    err = ioctl(fd, FIONREAD, &sz);
    if (err) {
        fprintf(stderr, "ioctl: %d %s\n", err, strerror(errno));
        return NULL;
    }
    char *got = NULL;
    if (sz == 0) {
        fprintf(stderr, "ERR: short read fd={%d}\n", client->fd);
        return NULL;
    }
    else if (sz < TCP_BUF_SZ - 1) {
        client->reader_str.sbuf_sz = sz;
        read_sz = read(fd, client->reader_str.sbuf, client->reader_str.sbuf_sz);
        got = client->reader_str.sbuf;
    } else {
        client->reader_str.buf_sz = sz;
        client->reader_str.buf = (char *)malloc(sz);
        got = client->reader_str.buf;
    }
    if (read_sz < 1 || read_sz > sz) {
        fprintf(stderr, "read: %d %s\n", err, strerror(errno));
        return NULL;
    } else {
        fprintf(stderr, "FROM: Client Address={%s}, port={%d} ", TCP_DEFAULT_IP, ntohs(client->addr.sin_port));
        fprintf(stderr, "GOT: fd={%d}, bytes={%d}, str={%s}\n", fd, sz, got);
    }
    pthread_mutex_lock(&client->writer_stick);
    sock_process(client);
    pthread_create(&client->writer_thread, NULL, socket_send, client);
    pthread_mutex_unlock(&client->reader_stick);

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return NULL;
}

/*
 *
 * */
void *
socket_send(void *vclient) {
    tcp_client client = (tcp_client)vclient;
    fprintf(stderr, "ERR: %s __start\n", __func__);

    fprintf(stderr, "SEND: Client Address={%s}, port={%d} ", TCP_DEFAULT_IP, ntohs(client->addr.sin_port));
    fprintf(stderr, "TO: fd={%d}, bytes={%zd}, str={%s}\n", client->fd, client->writer_str.sbuf_sz, client->writer_str.sbuf);

    ssize_t err = write(client->fd, client->writer_str.sbuf, TCP_BUF_SZ);
    if (err < 1 || err > client->writer_str.sbuf_sz) {
        fprintf(stderr, "write: %zd %s\n", err, strerror(errno));
    }
    pthread_mutex_unlock(&client->writer_stick);

    fprintf(stderr, "ERR: %s __end\n", __func__);
    return NULL;
}

int
t_remove_buf(tcp_str str) {
    if (str == NULL) { return 0; }
    t_remove_buf(str->next);
    free(str->buf);
    free(str);
    return 1;
}

int
t_remove_client(tcp_client curr) {
    t_remove_buf(curr->reader_str.next);
    if (curr->reader_str.buf) { free(curr->reader_str.buf); }
    t_remove_buf(curr->writer_str.next);
    if (curr->writer_str.buf) { free(curr->writer_str.buf); }
    if (tcpSocket.clients.curr == tcpSocket.clients.head) { /* head case */
        if (curr->next != NULL) { curr->next->prev = curr->prev;}
        tcpSocket.clients.head = curr->next;
    } else if (tcpSocket.clients.curr == tcpSocket.clients.tail) { /* tail case */
        if (curr->prev != NULL) { curr->prev->next = curr->next; }
        tcpSocket.clients.tail = curr->prev;
    } else { /* center case */
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
    }
    free(curr);
    tcpSocket.clients.size -= 1;
    return 0;
}

int
t_remove_fd_client_list(int fd) {
    for (tcpSocket.clients.curr = tcpSocket.clients.head; tcpSocket.clients.curr != NULL; tcpSocket.clients.curr = tcpSocket.clients.curr->next) {
        if (tcpSocket.clients.curr->fd == fd) { /* Remove this */
            t_remove_client(tcpSocket.clients.curr);
        }
    }
    return 0;
}

int
sock_process_recv(int fd) {
    /* TODO: swap this for a hashmap */
    tcp_client curr;
    for (curr = tcpSocket.clients.head; curr != NULL; curr = curr->next) {
        if (fd == curr->fd) { /* This client */
            /* Lock the reader here */
            int err;
            if (err = pthread_mutex_lock(&curr->reader_stick), err != 0) {
                fprintf(stderr, "ERR: mutex_lock %d %s\n", err, strerror(errno));
            }
            int sz = 0;
            err = ioctl(fd, FIONREAD, &sz);
            if (err) {
                fprintf(stderr, "ioctl: %d %s\n", err, strerror(errno));
                return -1;
            }
            if (sz > 0) { pthread_create(&curr->reader_thread, NULL, socket_recv, curr); }
            else { pthread_mutex_unlock(&curr->reader_stick); }
        }
    }
    return 0;
}

/*
 *
 * */
_Noreturn int
server_start() {
    fprintf(stderr, "ERR: %s __start\n", __func__);

    signal(SIGKILL, exit_func);
    signal(SIGTERM, exit_func);
    signal(SIGINT,  exit_func);

    socket_init();
    socket_bind();
    socket_listen();

    __attribute__((unused)) tcp_socket sock = &tcpSocket;

    while (1) {
        tcpSocket.poll_ret = poll(tcpSocket.poll_fds, tcpSocket.poll_max, -1);
        fprintf(stderr, "ERR: %s %d\n", "poll returned", tcpSocket.poll_ret);

        if (tcpSocket.poll_fds[0].revents & POLLIN) { /* New Connection */
            socket_accept();
            tcpSocket.poll_ret -= 1;
        }

        for (uint32_t i = 1; i <= tcpSocket.poll_max && tcpSocket.poll_ret > 0; i++) {
            if (tcpSocket.poll_fds[i].revents & POLLHUP) { /* Close Connection */
                t_remove_fd_client_list(tcpSocket.poll_fds[i].fd);
                tcpSocket.poll_fds[i].fd = -1;
                tcpSocket.poll_fds[i].events = 0;
                tcpSocket.poll_fds[i].revents = 0;
                tcpSocket.poll_ret -= 1;
            } else if (tcpSocket.poll_fds[i].revents & POLLIN) { /* Read Connection */
                sock_process_recv(tcpSocket.poll_fds[i].fd);
                tcpSocket.poll_fds[i].revents = 0;
            }
        }
    }
    /* Unreachable */
}
