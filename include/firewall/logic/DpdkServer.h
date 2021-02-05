//
// Created by tal on 28/01/2021.
//


#pragma once

#include "MBufServer.h"

#include <cstdint>

#include "pcapplusplus/Packet.h"
#include "pcapplusplus/DpdkDevice.h"
#include "pcapplusplus/DpdkDeviceList.h"
#include "pcapplusplus/IpAddress.h"
#include "pcapplusplus/EthLayer.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"
#include "pcapplusplus/ArpLayer.h"
#include "pcapplusplus/IcmpLayer.h"
#include "pcapplusplus/TcpLayer.h"
#include "pcapplusplus/UdpLayer.h"
#include "pcapplusplus/DhcpLayer.h"


class DpdkServer : public MBufServer {
public:

    struct DpdkServerArgs {

    public:

        std::uint32_t m_DpdkPortId = 0;
        std::uint16_t m_RxQueues = 1;
        std::uint16_t m_TxQueues = 1;
        std::uint16_t m_MaxBurstSize = 64;
        std::uint16_t m_MBufPoolSize = 16 * 1024 - 1;

    };

    class DpdkInitializationError : public std::runtime_error {

    public:

        DpdkInitializationError();

    };

    class UnknownDeviceException : public std::runtime_error {

    public:

        explicit UnknownDeviceException(std::uint32_t deviceId);

    };

    class MultiQueueException : public std::runtime_error {

    public:

        explicit MultiQueueException(std::uint16_t rxQueues, std::uint16_t txQueues);

    };

    explicit DpdkServer(DpdkServerArgs &args);

    virtual ~DpdkServer();

protected:

    virtual void Process(const pcpp::Packet &packet) = 0;

    virtual void HandleRequest(const pcpp::Packet &packet) = 0;

private:

    void HandleServerRunning() override;

protected:

    pcpp::DpdkDevice *m_Device;

};