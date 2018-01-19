#pragma once
#include "MovieGraph.hpp"
#include <string>

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace Core 
{
    public ref class Wrapper
    {
    public:
        void Load();
        List<String^>^ GetAllMovieNames();

        !Wrapper();
        ~Wrapper();

        Tuple<List<String^>^, int>^ GetShortestPath(String^ source, String^ destination, bool removeIsolated, double reserveProb, int reserveHigherThan);
        void GetMinimalSpanningTree(String^ root);
        List<List<String^>^>^ GetStronglyConnectedComponents(int threshold);
        void GetGraph(bool removeIsolated, double reserveProb, int reserveHigherThan);

        Dictionary<String^, double>^ GetAllBetweenness();
        Dictionary<String^, double>^ GetAllCloseness();
    private:
        MovieGraph * _movieGraph = nullptr;
        static std::wstring StringToWstring(String^ source);
    };
}