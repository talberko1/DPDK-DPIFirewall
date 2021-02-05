//
// Created by tal on 31/01/2021.
//


#include "firewall/filters/UDPFilter.h"


UDPFilter::UDPFilter(UDPFilter::Policy *policyStruct) : BaseProtocolFilter("UDP", policyStruct) {}

bool UDPFilter::Filter(const pcpp::Packet &packet) {

    if (packet.isPacketOfType(pcpp::UDP)) {

        auto *tcpLayer = packet.getLayerOfType<pcpp::UdpLayer>();

        auto *policy = ((UDPFilter::Policy *) m_PolicyStruct);

        auto *header = tcpLayer->getUdpHeader();

        if (header->length < policy->m_MinimumLength) return false;
        if (header->length > policy->m_MaximumLength) return false;

        bool srcPortValid = false;
        bool dstPortValid = false;

        for (std::uint16_t i = 0; i < policy->m_SrcPortAmount && !srcPortValid; i++)
            if (*(policy->m_SrcPorts + i) == header->portSrc)
                srcPortValid = true;

        for (std::uint16_t i = 0; i < policy->m_DstPortAmount && !dstPortValid; i++)
            if (*(policy->m_DstPorts + i) == header->portDst)
                dstPortValid = true;

        return srcPortValid && dstPortValid;

    }

    return false;
}
