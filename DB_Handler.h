//
// Created by dantroll on 16/06/19.
//

#ifndef SERVER_MYINVLIB_DB_HANDLER_H
#define SERVER_MYINVLIB_DB_HANDLER_H

#include "json.h"
#include <iostream>

using json = nlohmann::json;
using namespace std;

class DB_Handler {

public:

    void init();

    void INSERT(string, json);

    void UPDATE(string, json);

    void DELETE(string, json);

    void SELECT(string, json);

private:
    json DB;

};


#endif //SERVER_MYINVLIB_DB_HANDLER_H
