// Core.cpp : Defines the entry point for the console application.
//

// This file is used to export user network.

#using "UtilitiesSharp.dll"
using namespace System;

int main()
{
    UtilitiesSharp::UtilitiesSharp::BuildUserGraph(
        gcnew String(L"../Build/Assets/movie.csv"),gcnew String(L"../Build/Assets/user.csv")
    );
    return 0;
}
