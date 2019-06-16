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

void DB_Handler::INSERT(string img, string prefix, string data) {
    this->counter++;
    DB["METADATA"] = data;
}

void DB_Handler::INSERT(string img, string prefix, int data) {
    this->counter++;
    string met = "METADATA" + to_string(this->counter);
    DB[met][prefix] = data;
}
