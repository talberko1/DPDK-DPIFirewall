//
// Created by tal on 10/29/20.
//

#include <cstdlib>
#include <string>
#include "PcapFileCreator.h"

void PcapFileCreator::createFileFromBytes(const std::string &fileName, std::uint8_t *stream, std::size_t length) {
    FILE *file = fopen(fileName.c_str(), "wb");
    fwrite(stream, 1, length, file);
    fclose(file);
}

std::size_t PcapFileCreator::getFileSize(const std::string &name) {
    FILE *file = fopen(name.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    std::size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    rewind(file);
    fclose(file);
    return fileSize;
}

std::uint8_t *PcapFileCreator::createByteStreamFromFile(const std::string &name) {
    FILE *pcap_file = fopen(name.c_str(), "rb");
    std::size_t size = getFileSize(name);
    auto *stream = new std::uint8_t[size];
    fread(stream, 1, size, pcap_file);
    return stream;
}
