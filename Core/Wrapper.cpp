#include "Wrapper.hpp"
#include "Utilities.hpp"
#include "Dijkstra.hpp"
#include "Prim.hpp"
#include "Tarjan.hpp"
#include "Centrality.hpp"
#include <vector>
#include <string>

using namespace std;

void Core::Wrapper::Load()
{
    _movieGraph = new MovieGraph(Utilities::BuildGraphFromFile(L"./Assets/movie.csv", L"./Assets/user.csv"));
}

List<String^>^ Core::Wrapper::GetAllMovieNames()
{
    List<String^>^ ret = gcnew List<String^>();
    auto allMovies = _movieGraph->GetAllMovies();
    for (const auto& movie : allMovies)
    {
        ret->Add(gcnew String(movie->MovieName.c_str()));
    }

    return ret;
}

Core::Wrapper::!Wrapper()
{
    delete _movieGraph;
    _movieGraph = nullptr;
}

Core::Wrapper::~Wrapper()
{
    delete _movieGraph;
    _movieGraph = nullptr;
}

Tuple<List<String^>^, int>^ Core::Wrapper::GetShortestPath(String^ source, String^ destination, bool removeIsolated, double reserveProb, int reserveHigherThan)
{
    vector<wstring> path;
    int weight;

    auto movieGraph = Dijkstra::GetShortestPath(*_movieGraph, path, weight, StringToWstring(source), StringToWstring(destination));
    movieGraph.ExportToJson(L"./Assets/graph.json",0,1,removeIsolated,true,reserveProb,reserveHigherThan);

    auto pathList = gcnew List<String^>;
    for (const auto &node : path)
    {
        pathList->Add(gcnew String(node.c_str()));
    }

    return gcnew Tuple<List<String^>^, int>(pathList, weight);
}

wstring Core::Wrapper::StringToWstring(String^ source)
{
    using namespace Runtime::InteropServices;
    auto chars = (const wchar_t*)(Marshal::StringToHGlobalUni(source)).ToPointer();
    wstring ret = chars;
    Marshal::FreeHGlobal(IntPtr((void*)chars));

    return ret;
}

void Core::Wrapper::GetMinimalSpanningTree(String^ root)
{
    auto rootName = StringToWstring(root);
    auto movieGraph = Prim::GetMinimalSpanningTree(*_movieGraph, rootName);
    movieGraph.ExportToJson(L"./Assets/graph.json", 0, 1, false, false);
}

List<List<String^>^>^ Core::Wrapper::GetStronglyConnectedComponents(int threshold)
{
    MovieGraph newMovieGraph;
    auto res = Tarjan::GetStronglyConnectedComponents(*_movieGraph, newMovieGraph, threshold);

    auto ret = gcnew List<List<String^>^>();

    for (const auto& component : res)
    {
        auto insertingComponent = gcnew List<String^>();
        for (const auto& movie : component)
        {
            insertingComponent->Add(gcnew String(movie->MovieName.c_str()));
        }

        ret->Add(insertingComponent);
    }

    newMovieGraph.ExportToJson(L"./Assets/graph.json");

    return ret;
}


void Core::Wrapper::GetGraph(bool removeIsolated, double reserveProb, int reserveHigherThan)
{
    _movieGraph->ExportToJson(L"./Assets/graph.json", 0, 1, removeIsolated, true, reserveProb, reserveHigherThan);
}


Dictionary<String^, double>^ Core::Wrapper::GetAllBetweenness()
{
    auto ans = Centrality::GetAllBetweenness(*_movieGraph);
    auto ret = gcnew Dictionary<String^, double>();

    for (const auto i : ans)
    {
        ret->Add(gcnew String(i.first->MovieName.c_str()), i.second);
    }

    return ret;
}

Dictionary<String^, double>^ Core::Wrapper::GetAllCloseness()
{
    unordered_map<wstring, double> ans;
    for (const auto i : _movieGraph->Movies)
    {
        auto closeness = Centrality::GetCloseness(*_movieGraph, i.first);
        ans[i.first] = closeness;
    }

    auto ret = gcnew Dictionary<String^, double>();
    for (const auto i : ans)
    {
        ret->Add(gcnew String(i.first.c_str()), i.second);
    }

    return ret;
}
