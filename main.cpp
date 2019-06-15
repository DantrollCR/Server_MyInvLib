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
#include "b64.c"


#define TRUE 1
#define FALSE 0
#define PUERTO 6969

using namespace std;

int main(int argc, char *argv[]) {
    for (int i = 0; i < 4; ++i) {
        string n = to_string(i);
        system(("mkdir -p Disks/disk" + n).c_str()); // note the slash after accounts!

    }
    Raid raid;
    char *path2 = "/home/dantroll/CLionProjects/Server_MyInvLib/prueba2.jpeg";

    char *path1 = "/home/dantroll/CLionProjects/Server_MyInvLib/prueba3.jpeg";
    char *name = "img_part";


    //raid.chunkFile(path2,name,2048);
    raid.joinFile(name, path1);
/*
    string d = "Daniel";
    unsigned char *a = (unsigned char *) "Daniel";
    int i;
    for (int j = 0; j < d.length(); ++j) {
        for (i = 0; i < 8; i++) {
            printf("%d", !!((a[j] << i) & 0x80));
        }
        printf(" ");
    }
*/


    return 0;

    Server server;
    //raid.copy_file(path1,path2);

    return 0;

}