//
// Created by tal on 30/01/2021.
//

#include <PcapFileCreator.h>
#include "firewall/DPIServer.h"

DPIServer::InvalidIpAddressException::InvalidIpAddressException(const std::string &address) : runtime_error(
        "Invalid IP address: " + address) {}

DPIServer::DPIServer(DPIServer::DPIServerArgs &args) : DpdkServer(
        args), m_DhcpEnabled(args.m_DhcpEnabled),
        m_PortNumber(args.m_ServerPortNumber),
        m_IpAddress(args.m_ServerIpAddress),
        m_LogWriter("unfiltered.pcap"),
        m_FilteredWriter("filtered.pcap")
        {
    if (!m_IpAddress.isValid()) throw InvalidIpAddressException(m_IpAddress.toString());
    std::cout << "[Firewall] MAC Address: " << m_Device->getMacAddress().toString() << std::endl;
    std::cout << "[Firewall] IP Address: " << m_IpAddress.toString() << std::endl;
    std::cout << "[Firewall] Running on port " << args.m_ServerPortNumber << std::endl;
}

DPIServer::~DPIServer() {
    std::cout << "[Firewall] destructor" << std::endl;
}

void DPIServer::HandleServerStartup() {
    std::cout << "[Firewall] Server starting..." << std::endl;
}

void DPIServer::HandleServerShutdown() {
    std::cout << "[Firewall] Server closing..." << std::endl;
}

bool DPIServer::IsDataLinkLayerRequest(const pcpp::Packet &request) {

    auto *ethernetLayer = request.getLayerOfType<pcpp::EthLayer>();
    return ethernetLayer && ethernetLayer->getDestMac() == m_Device->getMacAddress();

}

bool DPIServer::IsNetworkLayerRequest(const pcpp::Packet &request) {
    return true;
    if (m_IpAddress.isIPv4()) {
//        return m_IpAddress.getIPv4() == request.getLayerOfType<pcpp::IPv4Layer>()->getDstIpAddress();
    }
    if (m_IpAddress.isIPv6()) {
//        return m_IpAddress.getIPv6() == request.getLayerOfType<pcpp::IPv6Layer>()->getDstIpAddress();
    }
    return false;

}

bool DPIServer::IsTransportLayerRequest(const pcpp::Packet &request) const {

    if (request.isPacketOfType(pcpp::TCP)) {
        return m_PortNumber == be16toh(request.getLayerOfType<pcpp::TcpLayer>()->getTcpHeader()->portDst);
    }
    if (request.isPacketOfType(pcpp::UDP)) {
        return m_PortNumber == be16toh(request.getLayerOfType<pcpp::UdpLayer>()->getUdpHeader()->portDst);
    }
    return false;

}

bool DPIServer::IsApplicationLayerRequest(const pcpp::Packet &request) {

    return request.isPacketOfType(FIREWALL) &&
           FirewallLayer::FirewallOpcodes::FIREWALL_REQUEST == request.getLayerOfType<FirewallLayer>()->GetOpcode();

}

void DPIServer::Process(const pcpp::Packet &packet) {

    if (IsDataLinkLayerRequest(packet)) {
        if (packet.isPacketOfType(pcpp::ARP)) {

            HandleArp(packet.getLayerOfType<pcpp::ArpLayer>());

        } else if (packet.isPacketOfType(pcpp::DHCP)) {

            HandleDhcp(packet.getLayerOfType<pcpp::DhcpLayer>());

        } else if (IsNetworkLayerRequest(packet)) {
            if (packet.isPacketOfType(pcpp::ICMP)) {

                HandleIcmp(packet.getLayerOfType<pcpp::IcmpLayer>());

            } else if (IsTransportLayerRequest(packet)) {
                std::cout << "here" << std::endl;
                auto *tcp = packet.getLayerOfType<pcpp::TcpLayer>();
                std::cout << "Sender: " << tcp->getTcpHeader()->portSrc << std::endl;
                if (IsApplicationLayerRequest(packet)) {
                    auto *fw = packet.getLayerOfType<FirewallLayer>();
                    std::cout << "Sender: " << fw->GetOpcode() << std::endl;
                    std::cout << "Received request!" << std::endl;
                    HandleRequest(packet);
                }
            }
        }
    }

}

void DPIServer::Filter(const std::string &unfiltered, const std::string &filtered) {

}

void DPIServer::HandleRequest(const pcpp::Packet &request) {
    std::cout << "RECEIVED REQUEST!!!" << std::endl;
    pcpp::Packet response;

    CreateResponseFromRequest(request, response);

    auto *payloadLayer = request.getLayerOfType<pcpp::PayloadLayer>();

    PcapFileCreator::createFileFromBytes("unfiltered.pcap", payloadLayer->getPayload(), payloadLayer->getPayloadLen());

    Filter("unfiltered.pcap", "filtered.pcap");

    m_Device->sendPacket(response, 0);
}

void DPIServer::CreateResponseFromRequest(const pcpp::Packet &request, pcpp::Packet &response) {

    std::uint8_t *data = PcapFileCreator::createByteStreamFromFile("filtered.pcap");
    std::size_t length = PcapFileCreator::getFileSize("filtered.pcap");

    pcpp::Layer *responseEthernetLayer = AllocateResponseDataLinkLayer(request);
    pcpp::Layer *responseIpLayer = AllocateResponseNetworkLayer(request);
    pcpp::Layer *responseTransportLayer = AllocateResponseTransportLayer(request);
    pcpp::Layer *responseApplicationLayer = AllocateResponseApplicationLayer(request);
    pcpp::Layer *payloadLayer = new pcpp::PayloadLayer(data, length, false);

    response.addLayer(responseEthernetLayer, true);
    response.addLayer(responseIpLayer, true);
    response.addLayer(responseTransportLayer, true);
    response.addLayer(responseApplicationLayer, true);
    response.addLayer(payloadLayer, true);

}

pcpp::Layer *DPIServer::AllocateResponseDataLinkLayer(const pcpp::Packet &packet) {
    if (packet.isPacketOfType(pcpp::Ethernet)) {
        return new pcpp::EthLayer(m_Device->getMacAddress(), packet.getLayerOfType<pcpp::EthLayer>()->getSourceMac());
    }
    return nullptr;
}

pcpp::Layer *DPIServer::AllocateResponseNetworkLayer(const pcpp::Packet &packet) {

    if (m_IpAddress.isIPv4() && packet.isPacketOfType(pcpp::IPv4)) {
        return new pcpp::IPv4Layer(m_IpAddress.getIPv4(),
                                   packet.getLayerOfType<pcpp::IPv4Layer>()->getSrcIpAddress());
    }
    if (m_IpAddress.isIPv6() && packet.isPacketOfType(pcpp::IPv6)) {
        return new pcpp::IPv6Layer(m_IpAddress.getIPv6(),
                                   packet.getLayerOfType<pcpp::IPv6Layer>()->getSrcIpAddress());
    }
    return nullptr;

}

pcpp::Layer *DPIServer::AllocateResponseTransportLayer(const pcpp::Packet &packet) const {

    if (packet.isPacketOfType(pcpp::TCP)) {
        auto *requestTcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
        return new pcpp::TcpLayer(m_PortNumber, be16toh(requestTcpLayer->getTcpHeader()->portSrc));
    }
    if (packet.isPacketOfType(pcpp::UDP)) {
        auto *requestUdpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
        return new pcpp::UdpLayer(m_PortNumber, be16toh(requestUdpLayer->getUdpHeader()->portSrc));
    }
    return nullptr;

}

pcpp::Layer *DPIServer::AllocateResponseApplicationLayer(const pcpp::Packet &packet) {
    if (packet.isPacketOfType(FIREWALL)) {
        auto *requestFirewallLayer = packet.getLayerOfType<FirewallLayer>();
        return new FirewallLayer(FirewallLayer::FirewallOpcodes::FIREWALL_REPLY,
                                 requestFirewallLayer->GetTargetFilter());
    }
    return nullptr;
}

void DPIServer::HandleArp(pcpp::ArpLayer *arpLayer) {
    if (m_IpAddress.isIPv4() && m_IpAddress.isValid()) {
        if (arpLayer->getTargetIpAddr() == m_IpAddress.getIPv4()) {

            pcpp::MacAddress deviceMacAddress = m_Device->getMacAddress();
            pcpp::MacAddress originMacAddress = arpLayer->getSenderMacAddress();

            pcpp::IPv4Address originIpAddress = arpLayer->getSenderIpAddr();

            pcpp::EthLayer responseEthernetLayer(deviceMacAddress, originMacAddress);
            pcpp::ArpLayer responseArpLayer(pcpp::ARP_REPLY, deviceMacAddress, originMacAddress, m_IpAddress.getIPv4(),
                                            originIpAddress);

            pcpp::Packet packet;
            packet.addLayer(&responseEthernetLayer);
            packet.addLayer(&responseArpLayer);

            m_Device->sendPacket(packet, 0);
        }
    }
}

void DPIServer::HandleDhcp(pcpp::DhcpLayer *dhcpLayer) const {
    if (m_DhcpEnabled) {
        auto dhcpMessageType = dhcpLayer->getMesageType();
        switch (dhcpMessageType) {
            case pcpp::DHCP_OFFER:
                //SendRequest();
            case pcpp::DHCP_ACK:
                //assign IP address & local config

            case pcpp::DHCP_NAK:
                //SendDiscover();
            default:
                break;
        }
    }
}

void DPIServer::HandleIcmp(pcpp::IcmpLayer *icmpLayer) {
    if (m_IpAddress.isValid()) {
        if (m_IpAddress.isIPv4()) {
//            pcpp::IPv4Address deviceAddress = m_IpAddress.getIPv4();
            pcpp::IcmpMessageType icmpMessageType = icmpLayer->getMessageType();
            switch (icmpMessageType) {
                case pcpp::ICMP_INFO_REQUEST:
                    //SendIcmpEchoReply();
                case pcpp::ICMP_TIMESTAMP_REQUEST:
                    //SendIcmpTimestampReply();
                case pcpp::ICMP_ECHO_REQUEST:
                    //SendIcmpInfoRequest();
                case pcpp::ICMP_ADDRESS_MASK_REQUEST:
                    //SendIcmpAddressMaskRequest
                default:
                    break;
            }
        } else if (m_IpAddress.isIPv6()) {
//            pcpp::IPv6Address deviceAddress = m_IpAddress.getIPv6();
        }
    }
}
