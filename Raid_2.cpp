//
// Created by Daniel Acuña Mora on 15/06/19.
//
#include <iostream>
#include <fstream>
#include "Raid_2.h"
#include "json.h"
using namespace std;
using json = nlohmann::json;


void Raid_2::init(string path, DB_Handler *db) {
    this->DB = db;
    this->mainpath = path;
    /**
     * Esto crea un Directorio Disk, con 4 sub directorios, los cuales seràn los
     * discos virtuales.
     */
    for (int i = 1; i < 5; ++i) {
        string n = to_string(i);
        system(("mkdir -p " + path + "/Disk" + n).c_str()); // note the slash after accounts!

    }


}

void Raid_2::calculateParity(string name) {
    ifstream file;
    string fullname;
    string parityName = "/" + name + "_parity.dat";
    char *part1;
    char *part2;
    char *part3;
    char *parity;
    int fileSize;
    int fileSize1;
    int fileSize2;

    for (int j = 1; j <= 4; ++j) {
        int disco = j;

        std::cout << this->mainpath << std::endl;
        fullname.append(mainpath + "/Disk" + to_string(j) + "/" + name + "_" + to_string(disco) + ".dat");
        file.open(fullname, ios::in | ios::binary);
        fileSize = getSize(&file);
        std::cout << fileSize << std::endl;
        if (j == 1) {
            std::cout << "Text here" << std::endl;
            part1 = new char[fileSize];
            file.read(part1, fileSize);
            file.close();
            fullname.clear();
            std::cout << "Text here2" << std::endl;

        }
        if (j == 2) {
            std::cout << "LLegue" << std::endl;
            std::cout << fullname << std::endl;

            fileSize1 = fileSize;
            part2 = new char[fileSize];
            file.read(part2, fileSize);
            file.close();
            fullname.clear();

        } else if (j == 3) {
            std::cout << "Text here" << std::endl;
            part3 = new char[fileSize];
            file.read(part3, fileSize);
            file.close();
            fullname.clear();

        }
    }


    std::cout << fileSize1 << std::endl;
    parity = new char[fileSize1];

    for (int i = 0; i < fileSize1; ++i) {
        parity[i] = part1[i] ^ part2[i] ^ part3[i];
    }
    ofstream out;
    out.open(mainpath + "/Disk" + to_string(parityPosition) + parityName, ios::out | ios::binary);
    out.write(parity, fileSize1);
}

void Raid_2::recoverFile(int partId, string name) {
    ifstream file;
    string fullname;
    string parityName = "/" + name + "_parity.dat";
    char *part1;
    char *part2;
    char *part3;
    char *parity;
    int fileSize;
    int fileSize1;
    int fileSize2;
    for (int j = 1; j < 4; ++j) {
        int disco = j;
        std::cout << this->mainpath << std::endl;
        fullname.append(mainpath + "/Disk" + to_string(j) + "/" + name + "_" + to_string(disco) + ".dat");
        file.open(fullname, ios::in | ios::binary);
        fileSize = getSize(&file);
        std::cout << fileSize << std::endl;
        if (j == 1 && file.is_open()) {
            std::cout << "Abrir 1" << std::endl;
            part1 = new char[fileSize];
            file.read(part1, fileSize);
            file.close();
            fullname.clear();

        }
        if (j == 2 && file.is_open()) {
            std::cout << "Abrir 2" << std::endl;
            std::cout << fullname << std::endl;

            fileSize1 = fileSize;
            part2 = new char[fileSize];
            file.read(part2, fileSize);
            file.close();
            fullname.clear();

        } else if (j == 3 && file.is_open()) {
            std::cout << "Abrir 3" << std::endl;
            part3 = new char[fileSize];

            file.read(part3, fileSize);
            file.close();
            fullname.clear();

        }
        fullname.clear();
    }
    fullname.clear();
    fullname.append(mainpath + "/Disk" + to_string(4) + parityName);
    file.open(fullname, ios::in | ios::binary);
    parity = new char[fileSize1];
    file.read(parity, fileSize1);
    if (partId == 1) {
        part1 = new char[fileSize1];
        for (int i = 0; i < fileSize1; ++i) {
            part1[i] = part2[i] ^ part3[i] ^ parity[i];
        }
        ofstream out;
        out.open(mainpath + "/Disk1/" + name + "_1.dat", ios::out | ios::binary);
        out.write(part1, fileSize1);

    } else if (partId == 2) {
        part2 = new char[fileSize1];
        for (int i = 0; i < fileSize1; ++i) {
            part2[i] = part1[i] ^ part3[i] ^ parity[i];
        }
        ofstream out;
        out.open(mainpath + "/Disk2/" + name + "_2.dat", ios::out | ios::binary);
        out.write(part2, fileSize1);

    } else if (partId == 3) {
        part3 = new char[fileSize1];
        for (int i = 0; i < fileSize1; ++i) {
            part3[i] = part1[i] ^ part2[i] ^ parity[i];
        }
        ofstream out;
        out.open(mainpath + "/Disk3/" + name + "_3.dat", ios::out | ios::binary);
        out.write(part3, fileSize1);
    }

}

void Raid_2::rebuildFile(string chunkName) {
    string fileName;
    int pasa = 0;

    // Create our output file
    inicio:
    ofstream outputfile;
    outputfile.open("/home/dantroll/CLionProjects/Server_MyInvLib/" + chunkName + ".jpg", ios::out | ios::binary);

    // If successful, loop through chunks matching chunkName
    if (outputfile.is_open()) {
        bool filefound = true;
        int counter = 1;
        int fileSize = 0;

        while (filefound) {
            string count = to_string(counter);
            filefound = false;
            // Build the filename
            fileName.clear();
            fileName.append(mainpath + "/Disk" + count + "/" + chunkName + "_" + count + ".dat");

            // Open chunk to read
            ifstream fileInput;
            fileInput.open(fileName.c_str(), ios::ate | ios::binary);

            // If chunk opened successfully, read it and write it to
            // output file.
            if (fileInput.is_open()) {
                filefound = true;

                fileSize = getSize(&fileInput);
                char *inputBuffer = new char[fileSize];

                fileInput.read(inputBuffer, fileSize);
                outputfile.write(inputBuffer, fileSize);
                delete (inputBuffer);
                fileInput.close();
                counter++;
            } else if (counter == 4) {
                break;
            } else {
                std::cout << "La parte: " << count << " no existe, se va a recuperar." << std::endl;
                recoverFile(counter, chunkName);

                goto inicio;
            }
        }
    }
}

int Raid_2::getSize(ifstream *file) {
    file->seekg(0, ios::end);
    int fileSize = file->tellg();
    file->seekg(0, ios::beg);
    return fileSize;
}

void Raid_2::chunkFile(string chunkName, string path) {
    ifstream file;
    char *D;
    char *A;
    char *B;
    char *P;
    string fullChunkName;
    ofstream out;
    file.open(path, ios::in | ios::binary);
    int fileSize = getSize(&file);
    int chunkSize = (fileSize / 3) + 1;
    std::cout << chunkSize << std::endl;
    int counter = 1;
    int disco = 1;
    char buffer[chunkSize];

    while (!file.eof()) {
        // Build the fileSize file name. Usually drive:\\chunkName.ext.N
        // N represents the Nth fileSize
        fullChunkName.clear();
        fullChunkName.append(
                mainpath + "/Disk" + to_string(disco) + "/" + chunkName + "_" + to_string(counter) + ".dat");


        std::cout << fullChunkName << std::endl;
        // Open new fileSize file name for output

        out.open(fullChunkName, ios::out | ios::trunc | ios::binary);
        // If fileSize file opened successfully, read from input and
        // write to output fileSize. Then close.
        if (out.is_open()) {
            file.read(buffer, chunkSize);
            // gcount() returns number of bytes read from stream.
            out.write(buffer, file.gcount());
            out.close();

            counter++;
            disco++;
        }
    }



    //out.open("/home/dantroll/CLionProjects/Server_MyInvLib/P.dat",ios::out | ios::binary);
    //out.write(P, sizeof(P));






}