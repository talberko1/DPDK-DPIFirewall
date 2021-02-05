//
// Created by tal on 8/29/20.
//

#pragma once


#include "PcapFileDevice.h"

class PcapFileException : public std::exception {
private:
    const char *m_Message;
public:
    explicit PcapFileException(const char *message) : m_Message(message) {}

    const char *getMessage() {
        return m_Message;
    }
};

class PcapFileManager {
public:

    static void openPcapWriter(pcpp::PcapFileWriterDevice &writer);

    static void openPcapReader(pcpp::IFileReaderDevice *reader);
};
