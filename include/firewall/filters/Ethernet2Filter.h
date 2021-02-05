//
// Created by tal on 31/01/2021.
//


#pragma once


#include "BaseProtocolFilter.h"

#include "pcapplusplus/EthLayer.h"


class Ethernet2Filter : public BaseProtocolFilter {

public:

    struct Policy : public BasePolicy {
        std::uint16_t *m_EtherTypes;
        std::uint16_t m_EtherTypeAmount;
    };

public:

    explicit Ethernet2Filter(Policy* policyStruct);

    bool Filter(const pcpp::Packet &packet) override;

};