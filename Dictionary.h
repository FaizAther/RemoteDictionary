//
// Created by macos on 6/15/22.
//

#ifndef TCP_SOCKET_DICTIONARY_H
#define TCP_SOCKET_DICTIONARY_H

class Dictionary {
    int _key;
    int _val;
public:
    Dictionary(int key, int val);

    void setVal(int xkey, int xval);

    int getVal(int xkey) {
        if (xkey == _key) { return this->_val; }
        return ~0;
    }
};

#endif //TCP_SOCKET_DICTIONARY_H
