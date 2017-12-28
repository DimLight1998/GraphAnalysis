#include "MovieGraph.hpp"
#include <fstream>
#include <codecvt>

#using "UtilitiesSharp.dll"

using namespace std;
using namespace System;
using namespace System::Collections::Generic;

MovieGraph::MovieGraph(const MovieGraph& graph) : Movies(graph.Movies)
{
}

MovieGraph::MovieGraph(MovieGraph&& graph) noexcept : Movies(move(graph.Movies))
{
}

void MovieGraph::AddMovie(const wstring& name, const int category)
{
    Movies.insert(make_pair(name, new MovieNode(name, category)));
}

void MovieGraph::ExportToJson(
    const wstring& fileName, 
    const int weightThreshold,
    const double weightModificationFactor,
    const bool removeIsolated,
    const bool mergeEdges
)
{
    Dictionary<String^, int>^ movieNameAndCategory = gcnew Dictionary<String^, int>();
    for (const auto &i : Movies)
    {
        movieNameAndCategory->Add(gcnew String(i.first.c_str()), i.second->Category);
    }

    List<Tuple<String^, String^, int>^>^ connections = gcnew List<Tuple<String^, String^, int>^>();
    for (const auto &i : Movies)
    {
        auto movie = i.second;
        if (mergeEdges) 
        {
            for (const auto&j : movie->Connections)
            {
                auto anotherMovie = j.first;
                if (movie->MovieName < anotherMovie->MovieName)
                {
                    connections->Add(
                        gcnew Tuple<String^, String^, int>(
                            gcnew String(movie->MovieName.c_str()),
                            gcnew String(anotherMovie->MovieName.c_str()),
                            j.second
                            )
                    );
                }
            }
        }
        else
        {
            for(const auto&j :movie->Connections)
            {
                auto anotherMovie = j.first;
                connections->Add(
                    gcnew Tuple<String^, String^, int>(
                        gcnew String(movie->MovieName.c_str()),
                        gcnew String(anotherMovie->MovieName.c_str()),
                        j.second
                        )
                );
            }
        }
    }

    UtilitiesSharp::UtilitiesSharp::ExportGraphToJson(
        gcnew String(fileName.c_str()),
        weightThreshold,
        weightModificationFactor,
        removeIsolated,
        movieNameAndCategory,
        connections
    );
}

vector<MovieNode*> MovieGraph::GetAllMovies() const
{
    vector<MovieNode*> ret;
    ret.reserve(Movies.size());
    for(const auto&i:Movies)
    {
        ret.push_back(i.second);
    }

    return ret;
}
