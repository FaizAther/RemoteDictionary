 #include <iostream>
extern "C" {
    #include "tcp_server.h"
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    socket_init();
    return 0;
}
