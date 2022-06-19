//
// Created by macos on 6/15/22.
//

#include "tcp_client.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int
client_init(uint32_t port)
{
    memset(&tcpClient, 0, sizeof(tcpClient));

    tcpClient.addr_sz = sizeof(struct sockaddr_in);
    tcpClient.addr.sin_family = AF_INET;
    tcpClient.addr.sin_addr.s_addr = inet_addr(TCP_DEFAULT_IP);
    tcpClient.addr.sin_port = htons(port);

    fprintf(stdout, "Socket Address={%s}, port={%d}\n", TCP_DEFAULT_IP, ntohs(tcpClient.addr.sin_port));

    tcpClient.err = tcpClient.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpClient.fd < 0) {
        fprintf(stderr, \
			"socket: %d %s\n", \
			tcpClient.fd, strerror(errno));
        goto FRESH_PEER_BAD;
    }
    tcpClient.err = connect(tcpClient.fd, (struct sockaddr *)&tcpClient.addr, tcpClient.addr_sz);
    if (tcpClient.err != 0) {
        fprintf(stderr, \
			"connect: %d %s\n", \
			tcpClient.err, strerror(errno));
        goto FRESH_PEER_BAD;
    }
FRESH_PEER_BAD:
    return tcpClient.err;
}

_Noreturn int
client_handle() {
    char buf[TCP_BUF_SZ] = {0};
    while (1) {
        memset(buf, 0, TCP_BUF_SZ);
        tcpClient.err = read(STDIN_FILENO, buf, TCP_BUF_SZ);
        if (tcpClient.err <= 0) {
            fprintf(stderr, "ERR: read_IN %d %s\n", tcpClient.err, strerror(errno));
            continue;
        }
        putchar('\n');
        tcpClient.err = write(tcpClient.fd, buf, strlen(buf));
        if (tcpClient.err <= 0) {
            fprintf(stderr, "ERR: write_SOCK %d %s\n", tcpClient.err, strerror(errno));
            continue;
        }
        tcpClient.err = read(tcpClient.fd, buf, TCP_BUF_SZ);
        if (tcpClient.err <= 0) {
            fprintf(stderr, "ERR: read_SOCK %d %s\n", tcpClient.err, strerror(errno));
            continue;
        }
        tcpClient.err = write(STDOUT_FILENO, buf, strlen(buf));
        if (tcpClient.err <= 0) {
            fprintf(stderr, "ERR: write_OUT %d %s\n", tcpClient.err, strerror(errno));
            continue;
        }
    }
}