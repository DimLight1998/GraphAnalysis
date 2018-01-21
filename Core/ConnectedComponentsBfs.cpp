#include "ConnectedComponentsBfs.hpp"
#include <queue>
#include "MovieGraph.hpp"

unordered_map<std::wstring, int> ConnectedComponentsBfs::GetConnectedComponentsBfs(const MovieGraph & movieGraph, MovieGraph & out, int threshold)
{
    unordered_map<std::wstring, int> ret;
    for (const auto& item : movieGraph.Movies)
    {
        ret.insert(make_pair(item.first, -1));
    }

    int kind = 1;
    queue<MovieNode*> movieQueue;

    bool finish = false;
    while (!finish)
    {
        finish = true;
        while (!movieQueue.empty())
        {
            auto node = movieQueue.front();
            movieQueue.pop();
            auto connections = node->Connections;

            for (const auto& connection : connections)
            {
                if (connection.second >= threshold && ret[connection.first->MovieName] == -1)
                {
                    movieQueue.push(connection.first);
                    ret[connection.first->MovieName] = kind;
                }
            }
        }

        kind++;
        for (auto& item : ret)
        {
            if (item.second == -1)
            {
                finish = false;
                item.second = kind;
                movieQueue.push(movieGraph.Movies.at(item.first));
            }
        }
    }
    return ret;
}
