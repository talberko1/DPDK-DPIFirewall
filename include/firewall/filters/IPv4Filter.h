//
// Created by tal on 31/01/2021.
//


#pragma once


#include "BaseProtocolFilter.h"

#include "pcapplusplus/IPv4Layer.h"


class IPv4Filter : public BaseProtocolFilter {

public:

    struct Policy : public BasePolicy {
        std::uint8_t m_Version;
        std::uint8_t m_MinimumIHL;
        std::uint8_t *m_DscpValues;
        std::uint8_t m_DscpValueAmount;
        std::uint8_t *m_OptionTypeValues;
        std::uint8_t m_OptionTypeAmount;
        std::uint16_t m_MinimumTotalLength;
        std::uint16_t m_MaximumFragmentOffset;
    };

public:

    explicit IPv4Filter(Policy* policyStruct);

    bool Filter(const pcpp::Packet &packet) override;

};