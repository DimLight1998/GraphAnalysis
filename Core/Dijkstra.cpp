#include "Dijkstra.hpp"
#include <unordered_map>
#include <tuple>
#include <stack>
#include <iostream>
using namespace std;

void Dijkstra::SimpleDijkstra(
    const MovieGraph& graph,
    vector<wstring>& path,
    int& weight,
    const wstring& source,
    const wstring& destination
)
{
    unordered_map<MovieNode*, tuple<bool, int, MovieNode*>> nodes;
    auto sourceNode = graph.Movies.find(source)->second;
    auto expandingNode = sourceNode;
    nodes.insert(make_pair(expandingNode, make_tuple(true, 0, nullptr)));

    for (const auto& connection : expandingNode->Connections)
    {
        nodes.insert(make_pair(connection.first, make_tuple(false, connection.second, sourceNode)));
    }

    auto updated = false;
    while (true)
    {
        updated = false;
        auto min = INT_MAX;
        for (const auto& node : nodes)
        {
            if (get<0>(node.second) == false && get<1>(node.second) < min)
            {
                min = get<1>(node.second);
                expandingNode = node.first;
                updated = true;
            }
        }

        if (!updated)
        {
            break;
        }

        if (expandingNode->MovieName == destination)
        {
            break;
        }

        get<0>(nodes.find(expandingNode)->second) = true;

        auto connections = expandingNode->Connections;
        for (const auto& connection : connections)
        {
            auto destinationNode = connection.first;
            const auto destinationDistance = connection.second;

            const auto location = nodes.find(destinationNode);
            if (location == nodes.end())
            {
                nodes.insert(make_pair(destinationNode, make_tuple(false, min + destinationDistance, expandingNode)));
            }
            else if (get<1>(location->second) > min + destinationDistance)
            {
                get<1>(location->second) = min + destinationDistance;
                get<2>(location->second) = expandingNode;
            }
        }
    }

    path.clear();

    if (!updated)
    {
        weight = -1;
        return;
    }

    weight = get<1>(nodes.find(expandingNode)->second);

    auto next = expandingNode;

    stack<MovieNode*> nodeStack;

    while (next != nullptr)
    {
        nodeStack.push(next);
        next = get<2>(nodes.find(next)->second);
    }

    while (!nodeStack.empty())
    {
        path.push_back(nodeStack.top()->MovieName);
        nodeStack.pop();
    }

    wcout << 1;
}

void Dijkstra::ParallelDijkstra(
    const MovieGraph& graph,
    vector<wstring>& path,
    int& weight,
    const wstring& source,
    const wstring& destination
)
{
}
