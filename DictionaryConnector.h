//
// Created by macos on 6/20/22.
//

#ifndef TCP_SOCKET_DICTIONARYCONNECTOR_H
#define TCP_SOCKET_DICTIONARYCONNECTOR_H

#define TCP_BUF_SZ 1460

#ifdef __cplusplus
extern "C" {
#else
#endif

void Dictionary_process(char buf[TCP_BUF_SZ]);

#ifdef __cplusplus
}
#endif

#endif //TCP_SOCKET_DICTIONARYCONNECTOR_H
