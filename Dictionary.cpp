//
// Created by macos on 6/15/22.
//

#include "Dictionary.h"
#include <iostream>

Dictionary::Dictionary() = default;

DICT_ERR Dictionary::set(std::string xkey, std::string xval) {
    std::string cpy;
    cpy.append(xval);
    if (!map[xkey].empty()) {
        map.erase(xkey);
    }
    map[xkey] = cpy;
    return D_OK;
}

std::tuple<DICT_ERR, std::string> Dictionary::get(const std::string &key) {
    std::string val = map[key];
    DICT_ERR r_err = D_OK;
    if (val.empty()) {
        r_err = D_NOT_FOUND;
        f_gets += 1;
    } else {
        s_gets += 1;
    }
    n_gets += 1;
    return std::tuple<DICT_ERR, std::string>{r_err, val};
}

std::string Dictionary::stats() {
    std::string d_stats;
    d_stats.append("Dictionary Stats:\n");
    d_stats.append("\t total number of get operations ");
    d_stats += std::to_string(n_gets);
    d_stats.append("\t total number of successful get operations ");
    d_stats += std::to_string(s_gets);
    d_stats.append("\t total number of failed get operations ");
    d_stats += std::to_string(f_gets);
    d_stats += "\n";
    std::cout << d_stats;
    return d_stats;
}

std::string Dictionary::process(std::string query) {
    try {
        process_handle(query);
    } catch (const std::exception &) {
        return {"{}"};
    }
}


/*
 * TODO: FIX BAD FUNCTION
 * */
std::string Dictionary::process_handle(std::string query) {
    std::string query_str;
    query_str.append(query);
    query_str.erase(remove_if(query_str.begin(), query_str.end(), isspace), query_str.end());
    bool setOn = false;
    bool statOn = false;

    query_str.erase(std::remove(query_str.begin(), query_str.end(), '{'), query_str.end());
    size_t pos = query_str.find(',');
    std::string cmd = query_str.substr(0, pos);
    query_str.erase(0, pos + 1);
    pos = query_str.find(':');
    cmd.erase(0, pos + 2);
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '\"'), cmd.end());
    std::string key;
    std::string val;
    cmd.erase(std::remove(cmd.begin(), cmd.end(), '}'), cmd.end());
    if (cmd != getCmd(D_STS)) {
        pos = query_str.find('}');
        key = query_str.substr(0, pos + 1);
        if (cmd == getCmd(D_SET)) {
            setOn = true;
            pos = key.find(',');
            val = key.substr(pos + 1, key.length());
            key.erase(pos, key.length());
            pos = key.find(':');
            key.erase(0, pos + 2);
            key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
            pos = val.find(':');
            val.erase(0, pos + 2);
            val.erase(std::remove(val.begin(), val.end(), '\"'), val.end());
            val.erase(std::remove(val.begin(), val.end(), '}'), val.end());
        } else {
            pos = key.find('}');
            key.erase(pos, key.length());
            pos = key.find(':');
            key.erase(0, pos + 2);
            key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
            key.erase(std::remove(key.begin(), key.end(), '}'), key.end());
        }
    } else { statOn = true; }

    std::cerr << "CMD: " << cmd;
    if (!statOn) { std::cerr << ", KEY: " << key; }
    if (setOn) { std::cerr << ", VAL: " << val; }
    std::cerr << std::endl;

    std::string response = {"{}"};
    if (cmd == getCmd(D_GET)) {
        std::tuple<DICT_ERR, std::string> retES = get(key);
        DICT_ERR retE = std::get<0>(retES);
        std::string retS = std::get<1>(retES);
        if (retE != D_OK) {
            response = "{";
            response += R"("ERROR":")";
            response += std::to_string(retE);
            response += "\"}";
        } else {
            response = "{";
            response += R"("KEY:")";
            response += key;
            response += R"("VALUE:")";
            response += retS;
            response += "\"}";
        }
    } else if (cmd == getCmd(D_SET)) {
        DICT_ERR retE = set(key, val);
        if (retE != D_OK) {
            response = "{";
            response += R"("ERROR":")";
            response += std::to_string(retE);
            response += "\"}";
        }
    } else if (cmd == getCmd(D_STS)) {
        response = stats();
    }
    return response;
}
