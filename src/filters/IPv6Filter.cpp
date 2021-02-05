//
// Created by tal on 31/01/2021.
//


#include "firewall/filters/IPv6Filter.h"

IPv6Filter::IPv6Filter(IPv6Filter::Policy *policyStruct) : BaseProtocolFilter("IPv6", policyStruct) {}

bool IPv6Filter::Filter(const pcpp::Packet &packet) {
    auto *ipv6Layer = packet.getLayerOfType<pcpp::IPv6Layer>();

    auto *policy = ((IPv6Filter::Policy *) m_PolicyStruct);

    auto *header = ipv6Layer->getIPv6Header();

    return header->ipVersion != policy->m_Version;
}
