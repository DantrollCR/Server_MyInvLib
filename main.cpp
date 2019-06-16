#include <stdio.h>
#include <iostream>
#include <string.h> //strlen
#include <stdlib.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fstream>

#include "Server.h"
#include "Raid.h"
#include "Raid_2.h"
#include "b64.c"
#include "json.h"
#include "DB_Handler.h"
// for convenience
using json = nlohmann::json;

using namespace std;

int main(int argc, char *argv[]) {
    DB_Handler db;
    db.init();
    Raid_2 *raid2;
    string path2 = "/home/dantroll/CLionProjects/Server_MyInvLib/prueba.jpg";
    char *name = "img_part1";

    string path1 = "/home/dantroll/CLionProjects/Server_MyInvLib/test.png";
    char *name2 = "img_part2";

    raid2->init("/home/dantroll/CLionProjects/Server_MyInvLib/Disks");
    //raid2.chunkFile(name,path2);
    //raid2.calculateParity(name);
    //raid2.chunkFile(name2,path1);
    //raid2.calculateParity(name2);
    //raid2.recoverFile(1,name);
    //raid2->rebuildFile(name);






    return 0;

    Server server;
    //raid.copy_file(path1,path2);

    return 0;

}