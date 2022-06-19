//
// Created by macos on 6/20/22.
//
#include <iostream>
extern "C" {
#include "tcp_client.h"
}

int
main(void)
{
    std::cout << "JELLO\n";

    client_init(5000);
    client_handle();
}