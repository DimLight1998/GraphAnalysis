#pragma once
#include "MovieGraph.hpp"
#include <string>

/**
 * \brief This class is used for calculating centrality.
 */
class Centrality
{
public:
    /**
     * \brief Get the betweenness centrality of all nodes in a graph.
     * \see http://blog.csdn.net/BetaRun/article/details/51168259
     * \param graph The graph to calculate centrality in.
     * \return Betweenness centrality of all nodes in the graph.
     */
    static std::unordered_map<MovieNode*, double> GetAllBetweenness(const MovieGraph& graph);

    /**
     * \brief Get the closeness centrality of a node in a graph.
     * Closeness is defined as Closeness[x]:=Sum[1/Distance[x,y]] for y!=x in the graph.
     * \param graph The graph to calculate centrality in.
     * \param movieName Name of the movie to calculate.
     * \return Closeness centrality of the node in the graph.
     */
    static double GetCloseness(const MovieGraph& graph, const std::wstring& movieName);
};
