//
// Created by macos on 6/15/22.
//

#include "Dictionary.h"
#include <iostream>

Dictionary::Dictionary() {}

DICT_ERR Dictionary::set(std::string xkey, std::string xval) {
    std::string cpy;
    cpy.append(xval);
    if (map[xkey] != "") {
        map.erase(xkey);
    }
    map[xkey] = cpy;
    return D_OK;
}

std::tuple<DICT_ERR, std::string> Dictionary::get(const std::string &key) {
    std::string val = map[key];
    DICT_ERR r_err = D_OK;
    if (val == "") {
        r_err = D_NOT_FOUND;
        f_gets += 1;
    } else {
        s_gets += 1;
    }
    n_gets += 1;
    return std::tuple<DICT_ERR, std::string>{r_err, val};
}

void Dictionary::stats() {
    std::cout << "Dictionary Stats:\n";
    std::cout << "\t total number of get operations " << n_gets;
    std::cout << "\t total number of successful get operations " << s_gets;
    std::cout << "\t total number of failed get operations " << f_gets;
}