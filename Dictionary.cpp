//
// Created by macos on 6/15/22.
//

#include "Dictionary.h"

Dictionary::Dictionary(int key, int val) {
    _key = key;
    setVal(key, val);
}

void Dictionary::setVal(int xkey, int xval) {
    if (xkey == _key) { _val = xval; }
}