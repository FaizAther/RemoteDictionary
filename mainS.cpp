 #include <iostream>
extern "C" {
    #include "tcp_server.h"
};
#include "Dictionary.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    server_start();
    Dictionary dict = Dictionary();
    dict.process(R"({"cmd":"SET", "key":"a", "val":"z"})");
    dict.process(R"({"cmd":"GET", "key":"a"})");
    dict.process(R"({"cmd":"STATUS")");
    dict.set("0", "0");
    dict.set("0", "5");
    fprintf(stderr, "ERR: %s\n", std::get<1>(dict.get("0")).c_str());
    std::string my_str = std::get<1>(dict.get("1"));
    return 0;
}
