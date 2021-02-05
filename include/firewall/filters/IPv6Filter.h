//
// Created by tal on 31/01/2021.
//


#pragma once


#include "BaseProtocolFilter.h"

#include "pcapplusplus/IPv6Layer.h"


class IPv6Filter : public BaseProtocolFilter {

public:

    struct Policy : public BasePolicy {
        std::uint8_t m_Version;
    };

public:

    explicit IPv6Filter(Policy* policyStruct);

    bool Filter(const pcpp::Packet &packet) override;

};