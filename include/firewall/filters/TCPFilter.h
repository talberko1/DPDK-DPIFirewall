//
// Created by tal on 31/01/2021.
//


#pragma once


#include "BaseProtocolFilter.h"

#include "pcapplusplus/TcpLayer.h"


class TCPFilter : public BaseProtocolFilter {

public:

    struct Policy : public BasePolicy {
        std::uint16_t *m_SrcPorts;
        std::uint16_t m_SrcPortAmount;
        std::uint16_t *m_DstPorts;
        std::uint16_t m_DstPortAmount;
        std::uint8_t m_MinimumDataOffset;
    };

public:

    explicit TCPFilter(Policy* policyStruct);

    bool Filter(const pcpp::Packet &packet) override;

};