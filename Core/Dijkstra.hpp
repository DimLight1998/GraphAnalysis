#pragma once
#include "MovieGraph.hpp"
#include <utility>
#include <string>
#include <vector>

class Dijkstra
{
public:
    static void SimpleDijkstra(
        const MovieGraph& graph,
        std::vector<std::wstring>& path,
        int& weight,
        const std::wstring& source,
        const std::wstring&destination
    );
    static void ParallelDijkstra(
        const MovieGraph& graph,
        std::vector<std::wstring>& path,
        int& weight,
        const std::wstring& source,
        const std::wstring&destination
    );
};
