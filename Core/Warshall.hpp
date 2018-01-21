#pragma once
#include "Warshall.hpp"
#include <string>
#include <vector>
#include "MovieGraph.hpp"
/**
* \brief This class is used for calculating shortest path.
*/
class Warshall
{
public:
    /**
    * \brief Get the shortest path between two movies, using Warshall.
    */
    static MovieGraph GetShortestPathLength(
        const MovieGraph& graph,
        int& weight,
        const std::wstring& source,
        const std::wstring& destination
    );

};
