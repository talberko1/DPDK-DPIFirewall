//
// Created by tal on 31/01/2021.
//


#include "firewall/filters/TCPFilter.h"


TCPFilter::TCPFilter(TCPFilter::Policy *policyStruct) : BaseProtocolFilter("TCP", policyStruct) {}

bool TCPFilter::Filter(const pcpp::Packet &packet) {

    if (packet.isPacketOfType(pcpp::TCP)) {
        auto *tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();

        auto *policy = ((TCPFilter::Policy *) m_PolicyStruct);

        auto *header = tcpLayer->getTcpHeader();

        if (header->dataOffset < policy->m_MinimumDataOffset) return false;

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
