//
// Created by tal on 30/01/2021.
//


#pragma once


#include "logic/DpdkServer.h"
#include "FirewallLayer.h"

#include "pcapplusplus/EthLayer.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"
#include "pcapplusplus/TcpLayer.h"
#include "pcapplusplus/UdpLayer.h"


class DPIServer : public DpdkServer {

public:

    struct DPIServerArgs : public DpdkServerArgs {

    public:

        bool m_DhcpEnabled = false;
        std::uint16_t m_ServerPortNumber;
        std::string m_ServerIpAddress;

    };

    class InvalidIpAddressException : public std::runtime_error {
    public:

        explicit InvalidIpAddressException(const std::string &address);

    };

    explicit DPIServer(DPIServerArgs &args);

    ~DPIServer() override;

protected:

    void HandleServerStartup() override;

    void HandleServerShutdown() override;

    void HandleRequest(const pcpp::Packet &packet) override;

private:

    void Process(const pcpp::Packet &packet) override;

    bool IsDataLinkLayerRequest(const pcpp::Packet &packet);

    bool IsNetworkLayerRequest(const pcpp::Packet &packet);

    bool IsTransportLayerRequest(const pcpp::Packet &packet) const;

    static bool IsApplicationLayerRequest(const pcpp::Packet &request);

    void HandleDhcp(pcpp::DhcpLayer *dhcpLayer) const;

    void HandleArp(pcpp::ArpLayer *arpLayer);

    void HandleIcmp(pcpp::IcmpLayer *icmpLayer);

    pcpp::Layer *AllocateResponseDataLinkLayer(const pcpp::Packet &packet);

    pcpp::Layer *AllocateResponseNetworkLayer(const pcpp::Packet &packet);

    pcpp::Layer *AllocateResponseTransportLayer(const pcpp::Packet &packet) const;

    static pcpp::Layer *AllocateResponseApplicationLayer(const pcpp::Packet &packet);

    void CreateResponseFromRequest(const pcpp::Packet &request, pcpp::Packet &response);

protected:

    bool m_DhcpEnabled;
    std::uint16_t m_PortNumber;
    pcpp::IPAddress m_IpAddress;

};