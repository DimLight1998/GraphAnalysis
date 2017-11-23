#pragma once
#include "MovieGraph.hpp"
#include <string>
#include <map>

class Utilities
{
public:
    static MovieGraph BuildGraphFromFile(const std::wstring& movieFile, const std::wstring& userFile);
};
