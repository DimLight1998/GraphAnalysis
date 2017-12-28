#pragma once
#include "MovieGraph.hpp"
#include <string>
#include <map>

class Utilities
{
public:
    /**
     * \brief Build a movie-user graph from files.
     * \param movieFile Path of the file of movies' data.
     * \param userFile Path of the file of users' data.
     * \return A graph containing the information loaded from the files. 
     */
    static MovieGraph BuildGraphFromFile(const std::wstring& movieFile, const std::wstring& userFile);
};
