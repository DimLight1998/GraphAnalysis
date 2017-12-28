#pragma once
#include <set>
#include "MovieNode.hpp"
#include "MovieGraph.hpp"

class Tarjan
{
public:

    /**
     * \brief Get the strongly connected components of a movie graph.
     * \param movieGraph The graph to get strongly connected components of.
     * \param threshold Threshold of the edge, only edges with weight not less than threshold will be counted.
     * \return A set of strongly connected components, which are represented as a set of nodes.
     */
    static std::set<std::set<MovieNode*>> GetStronglyConnectedComponents(const MovieGraph& movieGraph, int threshold = 1);



    /**
    * \brief Get the strongly connected components of a movie graph.
    * \param movieGraph The graph to get strongly connected components of.
    * \param [out] out The graph representing the remaining edges (edges having less weight are removed).
    * \param threshold Threshold of the edge, only edges with weight not less than threshold will be counted.
    * \return A set of strongly connected components, which are represented as a set of nodes.
    */
    static std::set<std::set<MovieNode*>> GetStronglyConnectedComponents(const MovieGraph& movieGraph, MovieGraph& out,int threshold = 1);


};
