#include "MovieGraph.hpp"

MovieGraph::MovieGraph(const MovieGraph& graph) : Movies(graph.Movies)
{
}

MovieGraph::MovieGraph(MovieGraph&& graph) noexcept : Movies(std::move(graph.Movies))
{
}

void MovieGraph::AddMovie(const std::wstring& name)
{
    Movies.insert(std::make_pair(name, new MovieNode(name)));
}
