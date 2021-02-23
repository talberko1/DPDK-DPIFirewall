//
// Created by tal on 29/01/2021.
//

#include "firewall/logic/MBufServer.h"


MBufServer::MBufServer(std::uint16_t packetBurstSize) : m_Running(false), m_MaxBurstSize(packetBurstSize) {
    std::cout << "[MBuf] Allocating " << m_MaxBurstSize * sizeof(pcpp::MBufRawPacket *) << " bytes." << std::endl;
    m_RawPackets = static_cast<pcpp::MBufRawPacket **>(::operator new(m_MaxBurstSize * sizeof(pcpp::MBufRawPacket *)));
    std::fill(m_RawPackets, m_RawPackets + m_MaxBurstSize, nullptr);
    if (!m_RawPackets)
        throw std::bad_alloc();
}

MBufServer::~MBufServer() {
    ::operator delete(m_RawPackets, m_MaxBurstSize * sizeof(pcpp::MBufRawPacket *));
    std::cout << "[MBuf] Deallocating " << m_MaxBurstSize * sizeof(pcpp::MBufRawPacket *) << " bytes."
              << std::endl;
}

void MBufServer::Start() {
    if (!m_Running) {
        HandleServerStartup();
        m_Running = true;
        m_ServerThread = std::thread(&MBufServer::Run, this);
    }
}

bool MBufServer::Running() {
    return m_Running;
}

void MBufServer::Run() {
    while (m_Running) {
        HandleServerRunning();
    }
}

void MBufServer::Close() {
    if (m_Running) {
        m_Running = false;
        if (m_ServerThread.joinable())
            m_ServerThread.join();
        HandleServerShutdown();
    }
}
