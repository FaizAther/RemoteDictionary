//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_DICTIONARY_H
#define TCP_SOCKET_DICTIONARY_H

#include <string>
#include <map>
#include <utility>
#include <tuple>
#include <string.h>

enum DICT_CMD {
    D_GET,
    D_SET,
    D_STS
};

enum DICT_ERR {
    D_OK,
    D_NOT_FOUND,
    D_FULL,
    D_INVALID
};

class Dictionary {
    std::map<std::string, std::string> map;
    uint32_t n_gets = 0;
    uint32_t s_gets = 0;
    uint32_t f_gets = 0;
public:
    Dictionary();

    DICT_ERR set(std::string xkey, std::string xval);

    std::tuple<DICT_ERR, std::string> get(const std::string& xkey);

    /*
     * 1. { "cmd":"GET", "key":"ABC" },
     * 2. { "cmd":"SET", "key":"ABC", "val":"XYZ" },
     * 3. { "cmd":"STATUS" }
     * */
    std::string process(std::string query);

    std::string process_handle(std::string query);

    std::string stats();

    static std::string getCmd(enum DICT_CMD d_cmd) {
        switch (d_cmd) {
            case D_GET:
                return "GET";
            case D_SET:
                return "SET";
            case D_STS:
                return "STATUS";
        }
        return {""};
    }
};

#endif //TCP_SOCKET_DICTIONARY_H
