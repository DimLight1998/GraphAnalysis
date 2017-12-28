#include "Prim.hpp"
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// todo has bug.

MovieGraph Prim::GetMinimalSpanningTree(const MovieGraph& graph, const wstring& rootName)
{
    if (graph.Movies.size() == 0)
    {
        return MovieGraph();
    }

    unordered_set<MovieNode*> settledNodes;
    unordered_map<MovieNode*, MovieNode*> nodeTree;
    unordered_map<MovieNode*, int> remainNodes;
    unordered_map<MovieNode*, int> distance;

    // Initialization.
    for (const auto& node : graph.Movies)
    {
        remainNodes.insert(make_pair(node.second, INT_MAX));
        nodeTree.insert(make_pair(node.second, nullptr));
    }

    remainNodes[graph.Movies.at(rootName)] = 0;

    // Build.
    while (remainNodes.size() != 0)
    {
        auto minNode = remainNodes.begin();
        for (auto iter = remainNodes.begin(); iter != remainNodes.end(); ++iter)
        {
            if (iter->second < minNode->second)
            {
                minNode = iter;
            }
        }

        if (minNode->second == INT_MAX)
        {
            minNode->second = 0;
        }

        auto expanding = minNode->first;
        remainNodes.erase(minNode);

        settledNodes.insert(expanding);
        for (const auto& connection : expanding->Connections)
        {
            auto movie = connection.first;
            if (settledNodes.find(movie) == settledNodes.end())
            {
                if (remainNodes.at(movie) > connection.second)
                {
                    remainNodes.at(movie) = connection.second;
                    nodeTree.at(movie) = expanding;
                }
            }
        }
    }

    // Begin constructing.
    unordered_map<MovieNode*, MovieNode*> nodeMap;

    MovieGraph ret;
    for (const auto& movie : graph.Movies)
    {
        auto newMovie = new MovieNode(movie.first, movie.second->Category);
        nodeMap.insert(make_pair(movie.second, newMovie));
        ret.Movies.insert(make_pair(newMovie->MovieName, newMovie));
    }

    for (const auto& connection : nodeTree)
    {
        if (connection.second != nullptr)
        {
            const auto sourceNode = nodeMap.at(connection.first);
            auto destinationNode = nodeMap.at(connection.second);
            auto distance = graph.Movies.at(sourceNode->MovieName)->Connections.at(connection.second);
            ret.Movies.at(sourceNode->MovieName)->Connections.insert(make_pair(destinationNode, distance));
        }
    }

    return ret;
}
