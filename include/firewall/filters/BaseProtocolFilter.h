//
// Created by tal on 31/01/2021.
//


#pragma once


#include <cstring>
#include <string>

#include "pcapplusplus/Packet.h"


class BaseProtocolFilter {

public:

    struct BasePolicy {};

public:

    BaseProtocolFilter(const char *protocolName, BasePolicy *policy);

    virtual bool Filter(const pcpp::Packet &packet) = 0;

protected:

    std::string m_ProtocolName;
    BasePolicy *m_PolicyStruct;

};