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

MovieGraph Prim::AnotherPrim(const MovieGraph & graph, const std::wstring & rootName)
{
    int nodeNumber = graph.Movies.size();
    if (nodeNumber == 0)
    {
        return MovieGraph();
    }

    unordered_set<MovieNode*> settledNodes;
    unordered_map<MovieNode*, MovieNode*> nodeTree;
    //remainNodes.second is the length of way to root. 
    unordered_map<MovieNode*, int> remainNodes;
    unordered_map<MovieNode*, int> lastChangeNodes;
    unordered_map<MovieNode*, int> distance;

    // Initialization.
    for (const auto& node : graph.Movies)
    {
        remainNodes.insert(make_pair(node.second, INT_MAX));
        nodeTree.insert(make_pair(node.second, nullptr));
    }

    MovieGraph ret;
    remainNodes[graph.Movies.at(rootName)] = 0;
    remainNodes.erase(graph.Movies.at(rootName));
    ret.AddMovie(rootName, graph.Movies.at(rootName)->Category);
    distance.insert(make_pair(graph.Movies.at(rootName), 0));

    int minLength = INT_MAX;
    auto minLengthNode = graph.Movies.at(rootName);

    for (auto & pNode : graph.Movies.at(rootName)->Connections)
    {
        if (pNode.second < INT_MAX)
        {
            remainNodes[pNode.first] = pNode.second;
            lastChangeNodes.insert(pNode);
            remainNodes.erase(pNode.first);
            nodeTree[pNode.first] = graph.Movies.at(rootName);
            if (pNode.second < minLength)
            {
                minLength = pNode.second;
                minLengthNode = pNode.first;
            }
        }
    }
    ret.AddMovie(minLengthNode->MovieName, minLengthNode->Category);
    distance.insert(make_pair(minLengthNode, minLength));
    lastChangeNodes.erase(minLengthNode);


    // Build.
    while (lastChangeNodes.size() != 0)
    {
        for (auto& pNode : minLengthNode->Connections)
        {
            if (pNode.second < remainNodes[pNode.first])
            {
                //updata last node
                nodeTree[pNode.first] = minLengthNode;
                //updata length
                remainNodes[pNode.first] = pNode.second;
                lastChangeNodes.insert(pNode);
                remainNodes.erase(pNode.first);
            }
        }

        minLength = INT_MAX;
        for (auto& pMinNode : lastChangeNodes)
        {
            if (pMinNode.second < minLength)
            {
                minLength = pMinNode.second;
                minLengthNode = pMinNode.first;
            }
        }
        ret.AddMovie(minLengthNode->MovieName, minLengthNode->Category);
        distance.insert(make_pair(minLengthNode, minLength));
        lastChangeNodes.erase(minLengthNode);
    }

    for (auto &pNode : nodeTree)
    {
        if (pNode.second != nullptr)
        {
            ret.Movies.at(pNode.second->MovieName)->Connections.insert(make_pair(ret.Movies.at(pNode.first->MovieName), distance.at(pNode.first)));
        }
    }
    return ret;
}
