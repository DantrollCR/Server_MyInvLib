//
// Created by dantroll on 09/06/19.
//

#include <fstream>
#include <iostream>
#include "Raid.h"
#include "base64.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iomanip>
#include "json.h"


using json = nlohmann::json;

using namespace std;

// Chunks a file by breaking it up into chunks of "chunkSize" bytes.
void Raid::chunkFile(char *fullFilePath, char *chunkName, unsigned long chunkSize) {
    ifstream fileStream;
    fileStream.open(fullFilePath, ios::in | ios::binary);
    fileStream.seekg(0, ios::end);
    int n = fileStream.tellg();
    fileStream.seekg(0, ios::beg);

    // File open a success
    std::cout << "Tamaño: " << n << std::endl;
    unsigned int chunk = n / 3 + 1;

    if (fileStream.is_open()) {
        ofstream output;
        int counter = 1;
        int disco = 0;

        string fullChunkName;

        // Create a buffer to hold each chunk
        char *buffer = new char[chunk];

        // Keep reading until end of file
        while (!fileStream.eof()) {
            if (disco >= 3) {
                disco = 0;
            }
            string num = to_string(disco);
            // Build the chunk file name. Usually drive:\\chunkName.ext.N
            // N represents the Nth chunk
            std::cout << num << std::endl;
            fullChunkName.clear();
            fullChunkName.append(
                    "/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk" + num + "/");
            fullChunkName.append(chunkName);
            std::string s = std::to_string(counter);
            fullChunkName.append("_" + s + ".data");

            // Convert counter integer into string and append to name.
            char intBuf[10];

            std::cout << fullChunkName << std::endl;
            // Open new chunk file name for output

            output.open(fullChunkName, ios::out | ios::trunc | ios::binary);
            disco++;
            // If chunk file opened successfully, read from input and
            // write to output chunk. Then close.
            if (output.is_open()) {
                fileStream.read(buffer, chunk);
                // gcount() returns number of bytes read from stream.
                output.write(buffer, fileStream.gcount());
                output.close();

                counter++;
            }
        }
        // Cleanup buffer
        delete (buffer);
        // Close input file stream.
        fileStream.close();
        calculateParity(chunkName);

        cout << "Chunking complete! " << counter - 1 << " files created." << endl;
    } else { cout << "Error opening file!" << endl; }
}

void Raid::recuperarArchivo(int parte, char *chunkName) {
    string file1;
    string file2;
    string file3;
    int fileSize = 4565;

    file1.clear();
    file1.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk0/img_part_1.data");

    file2.clear();
    file2.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk1/img_part_2.data");

    file3.clear();
    file3.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk2/img_part_3.data");

    // Open chunk to read
    ifstream fileInput;
    fileInput.open(file1.c_str(), ios::in | ios::binary);
    char *part1 = new char[4565];
    fileInput.read(part1, 4565);
    fileInput.close();


    ifstream fileInput2;
    fileInput2.open(file2.c_str(), ios::in | ios::binary);
    char *part2 = new char[fileSize];
    fileInput2.read(part2, fileSize);
    fileInput2.close();

    ifstream fileInput3;
    fileInput3.open(file3.c_str(), ios::in | ios::binary);
    char *part3 = new char[fileSize];
    fileInput3.read(part3, fileSize);
    fileInput3.close();
    file3.clear();
    file3.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk2/img_part_parity.data");

    fileInput3.open(file3.c_str(), ios::in | ios::binary);
    char *parityBuffer = new char[fileSize];
    fileInput3.read(parityBuffer, fileSize);
    fileInput3.close();

    file3.clear();
    file3.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk2/img_part_3_.data");

    fileInput3.open(file3.c_str(), ios::in | ios::binary);
    char *check3 = new char[fileSize];
    fileInput3.read(check3, fileSize);
    fileInput3.close();

    if (parte == 1) {
        std::cout << "Recuperando la parte" << parte << std::endl;
        for (int k = 0; k < fileSize; ++k) {
            part1[k] = part2[k] ^ part3[k] ^ parityBuffer[k];
        }
        file2.clear();
        file2.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk0/img_part_1.data");
        ofstream outputfile;
        outputfile.open(file2, ios::out | ios::binary);
        outputfile.write(part2, fileSize);
        outputfile.close();
    } else if (parte == 2) {

        std::cout << "Recuperando la parte" << parte << std::endl;
        for (int k = 0; k < fileSize; ++k) {
            part2[k] = part1[k] ^ part3[k] ^ parityBuffer[k];
        }

        file2.clear();
        file2.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk1/img_part_2.data");
        ofstream outputfile;
        outputfile.open(file2, ios::out | ios::binary);
        outputfile.write(part2, fileSize);
        outputfile.close();

    } else if (parte == 3) {
        unsigned int x = 1;
        std::cout << "Recuperando la parte" << parte << std::endl;
        for (int k = 0; k < fileSize; ++k) {
            part3[k] = part1[k] ^ part2[k] ^ parityBuffer[k];
        }
        for (int i = 0; i < fileSize; ++i) {
            if ((check3[i] ^ part3[i]) == x) {
                std::cout << "Error! en: " << i << std::endl;
            }
        }

        file3.clear();
        file3.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk2/img_part_3.data");
        ofstream outputfile;
        outputfile.open(file3, ios::out | ios::binary);
        outputfile.write(part3, fileSize);
        outputfile.close();
    }
}

void Raid::calculateParity(char *chunkName) {
    string file1;
    string file2;
    string file3;

    // Create our output file
    string fileOutput;
    fileOutput.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk3/");

    fileOutput.append(chunkName);
    fileOutput.append("_parity.data");

    ofstream outputfile;
    outputfile.open(fileOutput, ios::out | ios::binary);

    // If successful, loop through chunks matching chunkName
    if (outputfile.is_open()) {
        bool filefound = true;
        int counter = 1;
        int fileSize = 0;
        int disco = 0;

        if (filefound) {


            string num = to_string(disco);
            // Build the filename
            file1.clear();
            file1.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk0/img_part_1.data");

            file2.clear();
            file2.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk1/img_part_2.data");

            file3.clear();
            file3.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk2/img_part_3.data");

            // Open chunk to read
            ifstream fileInput;
            fileInput.open(file1.c_str(), ios::in | ios::binary);
            char *part1 = new char[fileSize];
            fileInput.read(part1, fileSize);
            fileInput.close();


            ifstream fileInput2;
            fileInput2.open(file2.c_str(), ios::in | ios::binary);
            char *part2 = new char[fileSize];
            fileInput2.read(part2, fileSize);
            fileInput2.close();

            ifstream fileInput3;
            fileInput3.open(file3.c_str(), ios::in | ios::binary);
            char *part3 = new char[fileSize];
            fileInput3.read(part3, fileSize);
            fileInput2.close();

            // If chunk opened successfully, read it and write it to
            // output file.
            std::cout << "Text here" << std::endl;
            if (!fileInput.is_open()) {
                std::cout << "Text here2" << std::endl;
                filefound = true;
                char *parityBuffer = new char[fileSize];


                for (int k = 0; k < fileSize; ++k) {
                    parityBuffer[k] = part1[k] ^ part2[k];
                }
                for (int i = 0; i < fileSize; ++i) {
                    parityBuffer[i] = parityBuffer[i] ^ part3[i];
                }
                std::cout << "Ya creè la paridad" << std::endl;

                outputfile.write(reinterpret_cast<const char *>(parityBuffer), fileSize);

                delete parityBuffer;

                counter++;
                outputfile.close();


            }
        }
    }
}

// Finds chunks by "chunkName" and creates file specified in fileOutput
void Raid::joinFile(char *chunkName, char *fileOutput) {
    string fileName;
    int pasa = 0;

    // Create our output file
    inicio:
    ofstream outputfile;
    outputfile.open(fileOutput, ios::out | ios::binary);

    // If successful, loop through chunks matching chunkName
    if (outputfile.is_open()) {
        bool filefound = true;
        int counter = 1;
        int fileSize = 0;
        int disco = 0;
        int pasadas = 0;
        bool stuck = false;
        while (filefound) {

            label:
            if (disco > 2) {
                disco = 0;

            }
            filefound = false;
            string num = to_string(disco);
            // Build the filename
            fileName.clear();
            fileName.append("/home/dantroll/CLionProjects/Server_MyInvLib/cmake-build-debug/Disks/disk" + num + "/");

            fileName.append(chunkName);
            std::string s = std::to_string(counter);
            fileName.append("_" + s + ".data");

            // Open chunk to read
            ifstream fileInput;
            fileInput.open(fileName.c_str(), ios::in | ios::binary);
            fileInput.seekg(0, ios::end);
            int n = fileInput.tellg();
            fileInput.seekg(0, ios::beg);

            // If chunk opened successfully, read it and write it to
            // output file.
            if (fileInput.is_open()) {
                filefound = true;
                stuck = false;

                fileSize = getFileSize(&fileInput);
                char *inputBuffer = new char[fileSize];

                fileInput.read(inputBuffer, fileSize);
                outputfile.write(inputBuffer, fileSize);
                delete (inputBuffer);
                disco = 0;
                fileInput.close();
                counter++;
                pasadas = 0;


            } else {
                stuck = true;
                disco++;
                pasadas++;
                if (pasadas > 2 & stuck == true) {
                    if (counter > 3) {
                        std::cout << "Finalizado con éxito." << std::endl;
                    } else {
                        std::cout << "La parte: " << counter << ", no existe. Recuperar o terminar programa."
                                  << std::endl;

                        recuperarArchivo(counter, chunkName);
                        pasa++;
                        if (pasa > 20) {
                            break;
                        }
                        goto inicio;
                    }

                } else {
                    goto label;
                }
            }


        }
        // Close output file.
        outputfile.close();


        cout << "File assembly complete!" << endl;
    } else { cout << "Error: Unable to open file for output." << endl; }

}

// Simply gets the file size of file.
int Raid::getFileSize(ifstream *file) {
    file->seekg(0, ios::end);
    int filesize = file->tellg();
    file->seekg(ios::beg);
    return filesize;
}

std::string Raid::getBytes(const char *path) {
    std::ifstream image;

    image.open(path, std::ios_base::binary);

    image.seekg(0, std::ios::end);
    int n = image.tellg();
    image.seekg(0, std::ios::beg);

    unsigned char *res = new unsigned char[n];
    for (int i = 0; i < n; i++)
        res[i] = '5';
    image.read(reinterpret_cast<char *>(res), n);
    std::string s = base64_encode(res, n);
    return s;

}

std::string Raid::getImage(std::string base64) {
    std::string s = base64_decode(base64);
    return s;


}

int Raid::copy_file(const char *srcfilename, const char *dstfilename) {
    long len;
    char *buf = NULL;
    FILE *fp = NULL;

    // Open the source file
    fp = fopen(srcfilename, "rb");
    if (!fp) return 0;

    // Get its length (in bytes)
    if (fseek(fp, 0, SEEK_END) != 0)  // This should typically succeed
    {                                 // (beware the 2Gb limitation, though)
        fclose(fp);
        return 0;
    }

    len = ftell(fp);
    rewind(fp);

    // Get a buffer big enough to hold it entirely
    buf = (char *) malloc(len);
    if (!buf) {
        fclose(fp);
        return 0;
    }

    // Read the entire file into the buffer
    if (!fread(buf, len, 1, fp)) {
        free(buf);
        fclose(fp);
        return 0;
    }

    fclose(fp);

    // Open the destination file
    fp = fopen(dstfilename, "wb");
    if (!fp) {
        free(buf);
        return 0;
    }
    // Write the entire buffer to file
    if (!fwrite(buf, len, 1, fp)) {
        free(buf);
        fclose(fp);
        return 0;
    }

    // All done -- return success
    fclose(fp);
    free(buf);
    return 1;
}