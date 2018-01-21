#pragma once
#include <unordered_map>
#include "MovieGraph.hpp"
using namespace std;

class ConnectedComponentsBfs
{
public:
    static unordered_map<std::wstring, int> GetConnectedComponentsBfs(const MovieGraph& movieGraph, MovieGraph& out, int threshold = 1);
};

