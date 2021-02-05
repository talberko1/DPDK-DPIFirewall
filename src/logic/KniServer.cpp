//
// Created by tal on 28/01/2021.
//

#include "firewall/logic/KniServer.h"

KniServer::KniModuleException::KniModuleException() : std::runtime_error("KNI module failed to initialize") {}

KniServer::KniServer(std::uint16_t packetBurstSize, KniServerArgs &args) : MBufServer(packetBurstSize) {
    std::cout << "[KniServer] Activating DPDK device..." << std::endl;
    pcpp::KniDeviceList &devices = pcpp::KniDeviceList::getInstance();
    if (!devices.isInitialized()) {
        throw KniModuleException();
    }
    m_Device = devices.getDeviceByPort(args.m_Config.portId);
    //m_Device = devices.createDevice(args.m_Config, args.m_MemPoolSize);
}

void KniServer::HandleServerStartup() {
    std::cout << "[KniServer] Server starting..." << std::endl;
}

void KniServer::HandleServerRunning() {
    std::uint16_t received = m_Device->receivePackets(m_RawPackets, m_MaxBurstSize);
    for (std::uint16_t i = 0; i < received; i++) {
        pcpp::Packet packet(m_RawPackets[i]);
        Process(packet);
    }
}

void KniServer::HandleServerShutdown() {
    std::cout << "[KniServer] Server closing..." << std::endl;
}

KniServer::~KniServer() {
    std::cout << "[KniServer] Closing DPDK device..." << std::endl;
}
