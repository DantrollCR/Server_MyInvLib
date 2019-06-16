//
// Created by dantroll on 16/06/19.
//

#include "DB_Handler.h"
#include "json.h"
#include <fstream>

using json = nlohmann::json;

void DB_Handler::init() {

    ifstream i("/home/dantroll/CLionProjects/Server_MyInvLib/Metadata_DB.json");
    json j;
    i >> j;
    this->DB = j;
    i.close();

}


