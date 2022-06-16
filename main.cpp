 #include <iostream>
extern "C" {
    #include "tcp_server.h"
};
#include "Dictionary.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    socket_init();
    Dictionary dict = Dictionary(0, 0);
    dict.setVal(0, 5);
    fprintf(stderr, "ERR: %d\n", dict.getVal(0));
    return 0;
}
