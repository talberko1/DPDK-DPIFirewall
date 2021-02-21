//
// Created by tal on 28/01/2021.
//

#include "firewall/logic/DpdkServer.h"

DpdkServer::DpdkInitializationError::DpdkInitializationError() : std::runtime_error("DPDK Initialization failed") {}

DpdkServer::UnknownDeviceException::UnknownDeviceException(std::uint32_t deviceId) : runtime_error(
        "Unknown DPDK port #" + std::to_string(deviceId)) {}

DpdkServer::MultiQueueException::MultiQueueException(std::uint16_t rxQueues, std::uint16_t txQueues) :
        runtime_error("Failed to open " + std::to_string(rxQueues) + " RX queues and " + std::to_string(txQueues) +
                      "TX queues") {}

DpdkServer::DpdkServer(DpdkServerArgs &args) : MBufServer(args.m_MaxBurstSize) {

    std::cout << "[DpdkServer] Creating server..." << std::endl;
    std::cout << "[DpdkServer] Activating DPDK device..." << std::endl;
    m_Device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(args.m_DpdkPortId);
    if (!m_Device)
        throw UnknownDeviceException(args.m_DpdkPortId);
    if (!m_Device->openMultiQueues(args.m_RxQueues, args.m_TxQueues))
        throw MultiQueueException(args.m_RxQueues, args.m_TxQueues);
    std::cout << "[DpdkServer] DPDK device active!" << std::endl;

}

DpdkServer::~DpdkServer() {
    if (m_Device->isOpened()) {
        std::cout << "[DpdkServer] Closing DPDK device..." << std::endl;
        m_Device->close();
    }

    std::cout << "[DpdkServer] Destroying server..." << std::endl;
}

void DpdkServer::HandleServerRunning() {
    std::uint16_t received = m_Device->receivePackets(m_RawPackets, m_MaxBurstSize, 0);
    for (std::uint16_t i = 0; i < received; i++) {
        pcpp::Packet packet(m_RawPackets[i]);
        Process(packet);
    }
}
