//
// Created by tal on 8/29/20.
//

#include "PcapFileManager.h"

void PcapFileManager::openPcapWriter(pcpp::PcapFileWriterDevice &writer) {
    if (!writer.open()) {
        throw PcapFileException("Cannot open the given file for writing");
    }
}

void PcapFileManager::openPcapReader(pcpp::IFileReaderDevice *reader) {
    if (!reader->open()) {
        throw PcapFileException("Cannot open the given file for reading");
    }
}