#pragma once
#include "MovieGraph.hpp"

/**
 * \brief This class is used for calculating minimal spanning tree.
 */
class Prim
{
public:
    /**
     * \brief Get the minimal spanning tree.
     * \param graph Graph to calculate the minimal spanning tree.
     * \param rootName Name of the root.
     * \return A graph whose edges are edges from the minimal spanning tree.
     */
    static MovieGraph GetMinimalSpanningTree(const MovieGraph& graph, const std::wstring& rootName);

    static MovieGraph AnotherPrim(const MovieGraph& graph, const std::wstring& rootName);
};
