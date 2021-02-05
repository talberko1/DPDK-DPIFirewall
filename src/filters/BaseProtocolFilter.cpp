//
// Created by tal on 31/01/2021.
//


#include "firewall/filters/BaseProtocolFilter.h"


BaseProtocolFilter::BaseProtocolFilter(const char *protocolName, BasePolicy *policy) : m_ProtocolName(protocolName),
                                                                                 m_PolicyStruct(policy) {}
