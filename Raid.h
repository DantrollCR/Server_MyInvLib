//
// Created by dantroll on 09/06/19.
//

#ifndef SERVER_MYINVLIB_RAID_H
#define SERVER_MYINVLIB_RAID_H


class Raid {
public:
    void checkFile(char *prefix, char *fileOutput);

    void recuperarArchivo(int parte, char *, int);

    void calculateParity(char *chunkName, int fileSize);

    std::string getImage(std::string base64);

    void chunkFile(char *fullFilePath, char *chunkName);

    std::string getBytes(const char *);

    void joinFile(char *chunkName, char *fileOutput);

    int getFileSize(std::ifstream *file);

    int copy_file(const char *srcfilename, const char *dstfilename);

};


#endif //SERVER_MYINVLIB_RAID_H
