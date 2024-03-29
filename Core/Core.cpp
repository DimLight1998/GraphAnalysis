// Core.cpp : Defines the entry point for the console application.
//

// This file is used to export user network.

#using "UtilitiesSharp.dll"
#include "MovieGraph.hpp"
#include "Utilities.hpp"
#include <vector>
using namespace std;
using namespace System;


int main()
{
    UtilitiesSharp::UtilitiesSharp::BuildUserGraph(
        gcnew String(L"../Build/Assets/movie.csv"), gcnew String(L"../Build/Assets/user.csv")
    );

    return 0;
}
