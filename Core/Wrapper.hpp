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

        Tuple<List<String^>^, int>^ GetShortestPath(String^ source, String^ destination);
        void GetMinimalSpanningTree(String^ root);
        List<List<String^>^>^ GetStronglyConnectedComponents(int threshold);
    private:
        MovieGraph * _movieGraph = nullptr;
        static std::wstring StringToWstring(String^ source);
    };
}