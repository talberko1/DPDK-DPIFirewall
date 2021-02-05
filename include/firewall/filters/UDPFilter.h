//
// Created by tal on 31/01/2021.
//


#pragma once


#include "BaseProtocolFilter.h"

#include "pcapplusplus/UdpLayer.h"


class UDPFilter : public BaseProtocolFilter {

public:

    struct Policy : public BasePolicy {
        std::uint16_t *m_SrcPorts;
        std::uint16_t m_SrcPortAmount;
        std::uint16_t *m_DstPorts;
        std::uint16_t m_DstPortAmount;
        std::uint16_t m_MinimumLength;
        std::uint16_t m_MaximumLength;
    };

public:

    explicit UDPFilter(Policy* policyStruct);

    bool Filter(const pcpp::Packet &packet) override;

};