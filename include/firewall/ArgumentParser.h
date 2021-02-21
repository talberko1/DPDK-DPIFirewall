//
// Created by tal on 06/02/2021.
//


#pragma once


#include <string>
#include <algorithm>


class ArgumentParser {

public:

    ArgumentParser(int argc, char *argv[]);

    bool OptionExists(const std::string &option) const;

    const char *GetOption(const std::string &option) const;

private:

    char **GetIterator(const std::string &option) const;

    char **GetEnd() const;

private:

    int m_Count;
    char **m_Arguments;

};