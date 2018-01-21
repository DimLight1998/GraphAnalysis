#pragma once
#include <string>
#include <unordered_map>


/**
 * \brief This class stores information about a single movie.
 */
class MovieNode
{
public:
    /**
     * \brief Name of the movie.
     */
    std::wstring MovieName;
    
    /**
     * \brief Category of the movie. This integer is determined when constructing the graph from the file.
     */
    int Category;

    /**
     * \brief Edges from the movie. The first parameter is the pointer to another movie,
     * and the second parameter is the weight (number of common audience).
     */
    std::unordered_map<MovieNode*, int> Connections;

    explicit MovieNode(const std::wstring& name,const int category);
};