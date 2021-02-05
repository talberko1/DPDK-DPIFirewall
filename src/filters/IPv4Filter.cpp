//
// Created by tal on 31/01/2021.
//


#include "firewall/filters/IPv4Filter.h"


IPv4Filter::IPv4Filter(IPv4Filter::Policy *policyStruct) : BaseProtocolFilter("IPv4", policyStruct) {}

bool IPv4Filter::Filter(const pcpp::Packet &packet) {

    if (packet.isPacketOfType(pcpp::IPv4)) {
        auto *ipv4Layer = packet.getLayerOfType<pcpp::IPv4Layer>();

        auto *policy = ((IPv4Filter::Policy *) m_PolicyStruct);

        auto *header = ipv4Layer->getIPv4Header();

        if (header->ipVersion != policy->m_Version) return false;
        if (header->internetHeaderLength < policy->m_MinimumIHL) return false;
        if (header->totalLength < policy->m_MinimumTotalLength) return false;
        if (header->fragmentOffset > policy->m_MaximumFragmentOffset) return false;

        std::uint8_t dscp = header->typeOfService & 0b11111100;

        for (std::uint8_t i = 0; i < policy->m_DscpValueAmount; i++)
            if (*(policy->m_DscpValues + i) == dscp)
                return true;
    }
    return false;

}
