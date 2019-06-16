//
// Created by dantroll on 15/06/19.
//

#ifndef SERVER_MYINVLIB_RAID_2_H
#define SERVER_MYINVLIB_RAID_2_H

#include "json.h"
#include <iostream>
#include "DB_Handler.h"

using json = nlohmann::json;
using namespace std;

class Raid_2 {
public:
    void init(string, DB_Handler *);

    void calculateParity(string);

    void chunkFile(string, string);

    void rebuildFile(string);

    int getSize(ifstream *file);

    void recoverFile(int, string);

private:
    DB_Handler *DB;
    string mainpath;
    int parityPosition = 4;
};


#endif //SERVER_MYINVLIB_RAID_2_H
