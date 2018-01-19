#pragma once
#include "MovieNode.hpp"

/**
 * \brief This class stores information about a movie-user graph.
 * Each vertex is a movie,
 * and the value of the edge between two vertexes represents the number of audience in common.
 */
class MovieGraph
{
public:
    /**
     * \brief Movies, first parameter is the name of it and second parameter is the pointer to it's node.
     */
    std::unordered_map<std::wstring, MovieNode*> Movies;

    MovieGraph() = default;

    MovieGraph(const MovieGraph& graph);

    MovieGraph(MovieGraph&& graph) noexcept;

    /**
     * \brief Add a new movie to the graph.
     * \param name Name of the movie.
     * \param category Category of the movie, should be an integer.
     */
    void AddMovie(const std::wstring& name, const int category);

    /**
     * \brief Export the instance to a json file for visualization.
     * \param fileName Name of the exporting file.
     * \param weightThreshold An integer controlling the weight of the edge.
     * Only weights greater or equal than this will be counted.
     * \param weightModificationFactor After filtering the edges by threshold,
     * you can use this to modify the weight (Should only be used for visualization).
     * \param removeIsolated Determine whether to remove the isolated nodes.
     * \param mergeEdges As the graph is implemented as a directed graph,
     * you can use this to determine whether to merge the duplicated edges.
     * (e.g. A->B and B->A will only be counted once as A->B.)
     * \param reserveProb A probability indicating whether the edge will be reserved.
     * \param reserveHigherThan A threshold, edges having higher weight will be reserved
     * ignoring reserveProb.
     */
    void ExportToJson(
        const std::wstring& fileName,
        const int weightThreshold = 0,
        const double weightModificationFactor = 1,
        const bool removeIsolated = false,
        const bool mergeEdges = true,
        const double reserveProb = 1,
        const int reserveHigherThan = 0
    );

    /**
     * \brief Get all movies in the graph.
     * \return A vector containing pointers to all movies in the graph.
     */
    std::vector<MovieNode*> GetAllMovies() const;


};
