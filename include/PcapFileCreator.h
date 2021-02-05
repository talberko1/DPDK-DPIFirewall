//
// Created by tal on 10/29/20.
//

#pragma once


#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

class PcapFileCreator {
public:
    static long getFileSize(const char *fileName);

    static void createFileFromBytes(const char *fileName, char *buffer, long fileSize);

    static char *createByteStreamFromFile(const char *fileName, long fileSize);
};

