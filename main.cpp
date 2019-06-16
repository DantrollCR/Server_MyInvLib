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

// for convenience
using json = nlohmann::json;

using namespace std;

int main(int argc, char *argv[]) {
    json j;

    Raid raid;
    Raid_2 raid2;
    string path2 = "/home/dantroll/CLionProjects/Server_MyInvLib/prueba2.jpeg";

    char *name = "img_part";

    raid2.init("/home/dantroll/CLionProjects/Server_MyInvLib/Disks");
    //raid2.chunkFile(name,path2);
    //raid2.calculateParity(name);
    //raid2.recoverFile(1,name);
    raid2.rebuildFile(name);


    j["ID"] = "Galerias";
    j["Dico1"] = {"Galeria 1.1", "Galeria 1.2", "Galeria 1.3"};
    j["Disco2"] = {"Galeria 2.1", "Galeria 2.2", "Galeria 2.3"};


    json j2;

    j2["ID"] = "Imagen";
    j2["Base64"] = "String largo del base64";
    j2["Nombre"] = "Los Locos programadores.img";
    j2["ETC"] = "LosDemas Datos";




    return 0;

    Server server;
    //raid.copy_file(path1,path2);

    return 0;

}