// Core.cpp : Defines the entry point for the console application.
//


#include "Utilities.hpp"
#include "Dijkstra.hpp"
using namespace std;

int main()
{
    const auto graph = Utilities::BuildGraphFromFile(LR"(C:\Users\hp\Desktop\movie.csv)",
        LR"(C:\Users\hp\Desktop\user.csv)");
    int weight;
    vector<wstring> path;
    Dijkstra::SimpleDijkstra(graph, path, weight, L"我是公务员", L"朝我心脏开枪");
    return 0;
}
