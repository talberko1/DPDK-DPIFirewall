//
// Created by tal on 31/01/2021.
//

#include "firewall/filters/Ethernet2Filter.h"

Ethernet2Filter::Ethernet2Filter(Ethernet2Filter::Policy *policyStruct) : BaseProtocolFilter("Ethernet-II",
                                                                                             policyStruct) {}

bool Ethernet2Filter::Filter(const pcpp::Packet &packet) {
    if (packet.isPacketOfType(pcpp::Ethernet)) {
        auto *ethernetLayer = packet.getLayerOfType<pcpp::EthLayer>();

        auto *policy = ((Ethernet2Filter::Policy*)m_PolicyStruct);

        std::uint16_t targetType = ethernetLayer->getEthHeader()->etherType;
        for (std::uint16_t i = 0; i < policy->m_EtherTypeAmount; i++)
            if (*(policy->m_EtherTypes + i) == targetType)
                return true;
    }
    return false;
}
