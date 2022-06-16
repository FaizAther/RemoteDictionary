//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_DICTIONARY_H
#define TCP_SOCKET_DICTIONARY_H

#include <string>
#include <map>
#include <utility>
#include <tuple>

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

    void stats();
};

#endif //TCP_SOCKET_DICTIONARY_H
