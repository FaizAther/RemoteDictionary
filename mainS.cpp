#include <iostream>
extern "C" {
    #include "tcp_server.h"
};
#include "Dictionary.h"

int main() {
    std::cout << "SERVER HELLO" << std::endl;
    server_start();
    return 0;
}
