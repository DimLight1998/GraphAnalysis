#include "Warshall.hpp"
#include <unordered_map>
#include <tuple>
#include <map>
#include <stack>
#include <iostream>
using namespace std;

MovieGraph Warshall::GetShortestPathLength(
    const MovieGraph& graph,
    int& weight,
    const wstring& source,
    const wstring& destination
)
{

    // 
    int nodesNumber = graph.Movies.size();
    auto weightGraph = new int*[nodesNumber];
    for (int i = 0; i < nodesNumber; i++)
    {
        weightGraph[i] = new int[nodesNumber];
        for (int j = 0; j < nodesNumber; j++)
        {
            weightGraph[i][j] = -1;
        }

    }
    map<MovieNode*, int> movieToID;
    map<int, MovieNode*> IDToMovie;
    int count = 0;
    for (auto& node : graph.Movies)
    {
        movieToID[node.second] = count;
        IDToMovie[count] = node.second;
        count++;
    }

    for (int i = 0; i < nodesNumber; i++)
    {
        for (auto& connectedNode : IDToMovie[i]->Connections)
        {
            weightGraph[i][movieToID[connectedNode.first]] = connectedNode.second;
        }

    }


    //main 
    for (int i = 0; i < nodesNumber; i++)
    {
        for (int j = 0; j < nodesNumber; j++)
        {
            for (int k = 0; k < nodesNumber; k++)
            {
                if (weightGraph[i][k] != -1 && weightGraph[k][j] != -1)
                {
                    if (weightGraph[i][j] == -1 || (weightGraph[i][k] + weightGraph[k][j]) < weightGraph[i][j])
                    {
                        weightGraph[i][j] = weightGraph[i][k] + weightGraph[k][j];
                    }
                }

            }
        }
    }


    int sourceID = movieToID[graph.Movies.at(source)];
    int destinationID = movieToID[graph.Movies.at(destination)];
    weight = weightGraph[sourceID][destinationID];

    return graph;
}

