#include "Centrality.hpp"
#include <unordered_set>
#include <queue>
#include <stack>
#include <iostream>
#include <omp.h>
using namespace std;

unordered_map<MovieNode*, double> Centrality::GetAllBetweenness(const MovieGraph& graph)
{
    auto allMovies = graph.GetAllMovies();

    unordered_map<MovieNode*, double> centralityBetweenness;
    centralityBetweenness.reserve(allMovies.size());
    for (const auto& i : allMovies)
    {
        centralityBetweenness[i] = 0.0;
    }

    auto progress = 0;

#pragma omp parallel for
    for (auto i = 0; i < static_cast<int>(allMovies.size()); i++)
    {
#pragma omp critical
        {
            progress++;
            cout << "Calculating betweenness #" << progress << endl;
        }

        const auto& targetMovie = allMovies[i];

        unordered_map<MovieNode*, unordered_set<MovieNode*>> previousNodes;
        previousNodes.reserve(allMovies.size());

        unordered_map<MovieNode*, unordered_set<MovieNode*>> potentialPreviousNodes;
        potentialPreviousNodes.reserve(allMovies.size());

        unordered_map<MovieNode*, int> distances;
        distances.reserve(allMovies.size());
        for (const auto& movie : allMovies)
        {
            distances[movie] = -1;
        }
        distances[targetMovie] = 0;

        unordered_map<MovieNode*, double> sigmas;
        sigmas.reserve(allMovies.size());
        for (const auto& movie : allMovies)
        {
            sigmas[movie] = 0.0;
        }
        sigmas[targetMovie] = 1.0;

        queue<MovieNode*> consideringMovies;
        consideringMovies.push(targetMovie);

        stack<MovieNode*> consideredMovies;

        // Dijkstra.
        while (!consideringMovies.empty())
        {
            while (!consideringMovies.empty())
            {
                auto currentMovie = consideringMovies.front();
                consideringMovies.pop();
                consideredMovies.push(currentMovie);

                // Update distances.
                for (const auto& connection : currentMovie->Connections)
                {
                    auto movieTo = connection.first;
                    const auto distance = connection.second;

                    if (distances[movieTo] == -1 || distances[movieTo] > distances[currentMovie] + distance)
                    {
                        distances[movieTo] = distances[currentMovie] + distance;
                        potentialPreviousNodes[movieTo].clear();
                        potentialPreviousNodes[movieTo].insert(currentMovie);
                    }
                    else if (distances[movieTo] == distances[currentMovie] + distance)
                    {
                        potentialPreviousNodes[movieTo].insert(currentMovie);
                    }
                }
            }

            // Update previous.
            auto shortestPath = -1;
            unordered_set<MovieNode*> updatingMovies;
            for (const auto& movie : allMovies)
            {
                if (movie == targetMovie)
                {
                    continue;
                }

                if (previousNodes[movie].empty() && distances[movie] != -1)
                {
                    if (shortestPath == -1 || shortestPath > distances[movie])
                    {
                        shortestPath = distances[movie];
                        updatingMovies.clear();
                        updatingMovies.insert(movie);
                    }
                    else if (shortestPath == distances[movie])
                    {
                        updatingMovies.insert(movie);
                    }
                }
            }

            for (const auto& updatingMovie : updatingMovies)
            {
                previousNodes[updatingMovie] = potentialPreviousNodes[updatingMovie];

                for (auto const& movie : previousNodes[updatingMovie])
                {
                    sigmas[updatingMovie] += sigmas[movie];
                }

                consideringMovies.push(updatingMovie);
            }
        }

        unordered_map<MovieNode*, double> deltas;
        deltas.reserve(allMovies.size());
        for (const auto& j : allMovies)
        {
            deltas[j] = 0.0;
        }

        while (!consideredMovies.empty())
        {
            auto top = consideredMovies.top();
            consideredMovies.pop();

            for (const auto& movie : previousNodes[top])
            {
                deltas[movie] = sigmas[movie] / sigmas[top] * (1 + deltas[top]);
            }

            if (top != targetMovie)
            {
#pragma omp critical
                {
                    centralityBetweenness[top] += deltas[top];
                }
            }
        }
    }

    for (auto& betweenness : centralityBetweenness)
    {
        betweenness.second /= 2.0;
    }

    return centralityBetweenness;
}

double Centrality::GetCloseness(const MovieGraph& graph, const wstring& movieName)
{
    auto allMovies = graph.GetAllMovies();
    const auto sourceMovie = graph.Movies.at(movieName);

    unordered_map<MovieNode*, int> distances;
    distances.reserve(allMovies.size());

    for (auto i : allMovies)
    {
        distances[i] = -1;
    }
    distances[sourceMovie] = 0;

    unordered_set<MovieNode*> consideredMovies;
    unordered_set<MovieNode*> buffer;

    buffer.insert(sourceMovie);
    while (!buffer.empty())
    {
        // Find minimal.
        MovieNode* nodeWithMinLength = nullptr;
        for (auto i : buffer)
        {
            if (nodeWithMinLength == nullptr || distances[i] < distances[nodeWithMinLength])
            {
                nodeWithMinLength = i;
            }
        }
        buffer.erase(nodeWithMinLength);

        // Expand this node.
        consideredMovies.insert(nodeWithMinLength);
        for (const auto& connection : nodeWithMinLength->Connections)
        {
            auto dest = connection.first;
            const auto distance = connection.second;

            if (consideredMovies.find(dest) == consideredMovies.end())
            {
                if (distances[dest] == -1 || distances[dest] > distances[nodeWithMinLength] + distance)
                {
                    distances[dest] = distances[nodeWithMinLength] + distance;
                    buffer.insert(dest);
                }
            }
        }
    }

    auto clossness = 0.0;
    for (const auto& i : distances)
    {
        if (i.second == -1 || i.second == 0)
        {
            continue;
        }

        clossness += 1 / i.second;
    }

    return clossness;
}
