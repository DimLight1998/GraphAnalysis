#pragma once
#include "MovieNode.hpp"

class MovieGraph
{
public:
    std::unordered_map<std::wstring, MovieNode*> Movies;

    MovieGraph() = default;

    MovieGraph(const MovieGraph& graph);

    MovieGraph(MovieGraph&& graph) noexcept;

    void AddMovie(const std::wstring& name);
};
