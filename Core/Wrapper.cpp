#include "Wrapper.hpp"
#include "Utilities.hpp"
#include "Dijkstra.hpp"
#include "Prim.hpp"
#include "Tarjan.hpp"
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

Tuple<List<String^>^, int>^ Core::Wrapper::GetShortestPath(String^ source, String^ destination)
{
    vector<wstring> path;
    int weight;

    auto movieGraph = Dijkstra::GetShortestPath(*_movieGraph, path, weight, StringToWstring(source), StringToWstring(destination));
    movieGraph.ExportToJson(L"./Assets/graph.json", 2);

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
    movieGraph.ExportToJson(L"./Assets/graph.json",0,1,false,false);
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

