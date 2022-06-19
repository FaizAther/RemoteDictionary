//
// Created by macos on 6/20/22.
//

#include "DictionaryConnector.h"
#include "Dictionary.h"

#define TCP_BUF_SZ 1460

#ifdef __cplusplus
extern "C" {
#endif

static Dictionary *dictionaryInst = NULL;

void lazyDictionary(void) {
    if (dictionaryInst == NULL) {
        dictionaryInst = new Dictionary();
    }
}

void Dictionary_process(char buf[TCP_BUF_SZ]) {
    lazyDictionary();
    std::string response = dictionaryInst->process(buf);
    memset(buf, 0, TCP_BUF_SZ);
    memcpy(buf, response.c_str(), TCP_BUF_SZ);
}

#ifdef __cplusplus
}
#endif