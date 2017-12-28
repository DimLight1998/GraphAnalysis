#pragma once
#include "MovieGraph.hpp"
#include <string>
#include <vector>

/**
 * \brief This class is used for calculating shortest path.
 */
class Dijkstra
{
public:
    /**
     * \brief Get the shortest path between two movies, using Dijkstra.
     * \param [in] graph The graph to be used.
     * \param [out] path The names of the movies on the shortest path calculated (including start and end).
     * If there are many shortest paths, one of them will be returned. An empty vector if there is no shortest path.
     * \param [out] weight The weight of the path in total. -1 if there is no shortest path.
     * \param [in] source Name of the movie of source node.
     * \param [in] destination Name of the movie of the destination node.
     * \return A graph for the convenience to visualize (and should be used for this only).
     */
    static MovieGraph GetShortestPath(
        const MovieGraph& graph,
        std::vector<std::wstring>& path,
        int& weight,
        const std::wstring& source,
        const std::wstring& destination
    );
};
