//
// Created by tal on 10/29/20.
//

#pragma once


#include <cstdint>
#include <cstdio>
#include <string>
#include <sys/stat.h>

class PcapFileCreator {
public:
    static std::size_t getFileSize(const std::string &fileName);

    static void createFileFromBytes(const std::string& fileName, std::uint8_t *stream, std::size_t fileSize);

    static uint8_t *createByteStreamFromFile(const std::string &fileName);
};

