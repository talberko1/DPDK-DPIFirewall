//
// Created by tal on 10/29/20.
//

#include <cstdlib>
#include "PcapFileCreator.h"

void PcapFileCreator::createFileFromBytes(const char *fileName, char *buffer, long fileSize) {
    FILE *file = fopen(fileName, "wb");
    fwrite(buffer, fileSize, 1, file);
    fclose(file);
}

long PcapFileCreator::getFileSize(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    rewind(file);
    fclose(file);
    return fileSize;
}

char *PcapFileCreator::createByteStreamFromFile(const char *name, long fileSize) {
    FILE *pcap_file = fopen(name, "rb");
    char *buffer = (char *) malloc(fileSize * sizeof(char));
    fread(buffer, fileSize, 1, pcap_file);
    return buffer;
}
