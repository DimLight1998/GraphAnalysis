#include "Tarjan.hpp"
#include <functional>
#include <algorithm>
#include <stack>
#include <set>
using namespace std;

set<set<MovieNode*>> Tarjan::GetStronglyConnectedComponents(const MovieGraph& movieGraph, const int threshold)
{
    set<set<MovieNode*>> retSet;

    auto allMovies = movieGraph.GetAllMovies();
    auto movieIndexMap = unordered_map<MovieNode*, int>();

    const auto size = allMovies.size();

    for (auto i = 0; i < static_cast<int>(size); i++)
    {
        movieIndexMap[allMovies[i]] = i;
    }

    auto checked = new bool[size];
    auto rootOf = new int[size];
    auto timeOfChecking = new int[size];
    auto isInStack = new bool[size];
    auto timer = 0;

    for (auto i = 0; i < static_cast<int>(size); i++)
    {
        checked[i] = isInStack[i] = false;
        timeOfChecking[i] = rootOf[i] = -1;
    }
    stack<int> indexStack;

    function<void(int)> tarjan =
        [=, &timer, &retSet, &allMovies, &tarjan, &movieIndexMap, &indexStack](int index)-> void
    {
        checked[index] = true;
        timer++;
        rootOf[index] = timeOfChecking[index] = timer;
        indexStack.push(index);
        isInStack[index] = true;
        for (const auto& connection : allMovies[index]->Connections)
        {
            if (connection.second >= threshold)
            {
                auto destinationMovieIndex = movieIndexMap[connection.first];
                if (!checked[destinationMovieIndex])
                {
                    tarjan(destinationMovieIndex);
                    rootOf[index] = min(rootOf[index], rootOf[destinationMovieIndex]);
                }
                else if (isInStack[destinationMovieIndex])
                {
                    rootOf[index] = min(rootOf[index], timeOfChecking[destinationMovieIndex]);
                }
            }
        }

        if (timeOfChecking[index] == rootOf[index])
        {
            set<MovieNode*> adding;
            auto top = 0;
            do
            {
                top = indexStack.top();
                indexStack.pop();
                isInStack[top] = false;
                adding.insert(allMovies[top]);
            } while (top != index);

            retSet.insert(adding);
        }
    };

    for (auto i = 0; i < static_cast<int>(size); i++)
    {
        if (!checked[i])
        {
            tarjan(i);
        }
    }

    delete[] isInStack;
    delete[] timeOfChecking;
    delete[] rootOf;
    delete[] checked;

    return retSet;
}

std::set<std::set<MovieNode*>> Tarjan::GetStronglyConnectedComponents
(const MovieGraph& movieGraph, MovieGraph& out, const int threshold)
{
    auto ret = GetStronglyConnectedComponents(movieGraph, threshold);

    // Clone graph.
    unordered_map<MovieNode*, MovieNode*> nodeMap;
    out.Movies.clear();
    for (const auto& item : movieGraph.Movies)
    {
        auto newMovie = new MovieNode(item.first, item.second->Category);
        nodeMap[item.second] = newMovie;
        out.Movies.insert(make_pair(newMovie->MovieName, newMovie));
    }

    for (const auto& item : movieGraph.Movies)
    {
        auto newMovie = out.Movies.at(item.second->MovieName);
        for (const auto& dest : item.second->Connections)
        {
            auto weight = dest.second;
            if (weight >= threshold)
            {
                newMovie->Connections.insert(make_pair(nodeMap.at(dest.first), weight));
            }
        }
    }

    return ret;
}
