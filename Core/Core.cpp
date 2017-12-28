// Core.cpp : Defines the entry point for the console application.
//

#include "Utilities.hpp"
#include "Dijkstra.hpp"
#include "Prim.hpp"
#include "Centrality.hpp"
#include "Tarjan.hpp"
#include "MovieGraph.hpp"
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;

int main()
{
    const auto graph = Utilities::BuildGraphFromFile(
        LR"(C:\Users\hp\Desktop\movie.csv)",
        LR"(C:\Users\hp\Desktop\user.csv)"
    );

//        int weight;
//        vector<wstring> path;
//        auto shortestPath = Dijkstra::GetShortestPath(graph, path, weight, L"a", L"i");
//        shortestPath.ExportToJson(L"graph.json", 1, 1, true);
//    
//        auto supportTree = Prim::GetMinimalSpanningTree(graph,);
//        supportTree.ExportToJson(L"graph.json", 0, 1, false, false);
    
//        auto betweenness = Centrality::GetAllBetweenness(graph);
//    
//        wcout << betweenness.size() << endl;
//        wofstream fout;
//        fout.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
//        fout.open(L"out.txt");
//        
//        for(const auto i:betweenness)
//        {
//            fout << i.first->MovieName << L' ' << i.second << endl;
//        }

    auto st = Prim::GetMinimalSpanningTree(graph, L"1");
    st.ExportToJson(L"xxxxx.json",0,1,false, false);
    auto ret = Tarjan::GetStronglyConnectedComponents(graph, 20);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 30);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 40);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 50);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 60);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 70);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 80);
    ret = Tarjan::GetStronglyConnectedComponents(graph, 90);

    return 0;
}
