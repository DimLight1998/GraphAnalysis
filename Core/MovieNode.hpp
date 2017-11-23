#pragma once
#include <string>
#include <unordered_map>

class MovieNode
{
public:
    std::wstring MovieName;
    std::unordered_map<MovieNode*, int> Connections;

    explicit MovieNode(const std::wstring& name);
};
