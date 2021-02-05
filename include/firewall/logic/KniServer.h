//
// Created by tal on 27/01/2021.
//

#pragma once


#include "MBufServer.h"

#include <iostream>

#include "pcapplusplus/Packet.h"
#include "pcapplusplus/MBufRawPacket.h"
#include "pcapplusplus/KniDevice.h"
#include "pcapplusplus/KniDeviceList.h"


class KniServer : public MBufServer {

public:

    struct KniServerArgs {

        pcpp::KniDevice::KniDeviceConfiguration m_Config;
        const std::size_t m_MemPoolSize{};

    };

    class KniModuleException : std::runtime_error {
    public:

        KniModuleException();

    };

public:

    KniServer(std::uint16_t packetBurstSize, KniServerArgs &args);

    virtual ~KniServer();

protected:

    void HandleServerStartup() override;

    void HandleServerShutdown() override;

    virtual void Process(const pcpp::Packet &packet) = 0;

private:

    void HandleServerRunning() override;

protected:

    pcpp::KniDevice *m_Device;

};
