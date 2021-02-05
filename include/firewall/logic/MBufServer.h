//
// Created by tal on 29/01/2021.
//


#pragma once


#include <thread>
#include <atomic>
#include <cstdint>
#include <iostream>

#include "pcapplusplus/MBufRawPacket.h"


class MBufServer {

public:

    explicit MBufServer(std::uint16_t packetBurstSize);

    ~MBufServer();

    void Start();

    bool Running();

    void Close();

protected:

    virtual void HandleServerStartup() = 0;

    virtual void HandleServerRunning() = 0;

    virtual void HandleServerShutdown() = 0;

protected:

    std::atomic<bool> m_Running;
    std::thread m_ServerThread;
    std::uint16_t m_MaxBurstSize;
    pcpp::MBufRawPacket **m_RawPackets;

private:

    void Run();

};