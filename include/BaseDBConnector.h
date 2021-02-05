//
// Created by tal on 03/01/2021.
//

#pragma once


class BaseDBConnector {
private:
    const char *m_Uri;
public:
    explicit BaseDBConnector(const char *uri);

};
