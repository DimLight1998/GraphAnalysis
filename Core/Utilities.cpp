#include "Utilities.hpp"
#include <omp.h>
#include <string>
#include <fstream>
#include <map>
#include <codecvt>
#include <locale>
#include <iostream>
#include <unordered_set>
using namespace std;

MovieGraph Utilities::BuildGraphFromFile(const wstring& movieFile, const wstring& userFile)
{
    unordered_set<wstring> movies;
    unordered_set<wstring> lines;
    unordered_multimap<wstring, wstring> movieUserMap;
    unordered_multimap<wstring, wstring> userMovieMap;

    MovieGraph movieGraph;

    wfstream movieFileIn;
    movieFileIn.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
    wstring movieFileLine;

    wfstream userFileIn;
    userFileIn.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
    wstring userFileLine;

    auto progress = 0;

#pragma omp parallel sections
    {
        // Read all movie names.
#pragma omp section
        {
            unordered_map<wstring, int> catagories;
            unordered_map<wstring, int> movieCategoryMap;
            auto categoryCount = 0;
            movieFileIn.open(movieFile);
            while (getline(movieFileIn, movieFileLine))
            {
                const auto firstComma = movieFileLine.find_first_of(L',');
                const auto lastComma = movieFileLine.find_last_of(L',');

                auto category = movieFileLine.substr(0, firstComma);
                const auto result = catagories.find(category);
                if (result == catagories.end())
                {
                    catagories.insert(make_pair(category, ++categoryCount));
                }

                movieFileLine = movieFileLine.substr(firstComma + 1, lastComma - firstComma - 1);
                movies.insert(movieFileLine);
                movieCategoryMap.insert(make_pair(movieFileLine, catagories.find(category)->second));
            }

            for (const auto& i : movies)
            {
                movieGraph.AddMovie(i, movieCategoryMap.find(i)->second);
            }
        }

        // Read all records.
#pragma omp section
        {
            userFileIn.open(userFile);
            while (getline(userFileIn, userFileLine))
            {
                if (++progress % 1000 == 0)
                {
                    wcout << "Reading record #" << progress << endl;
                }

                lines.insert(userFileLine);
            }
            wcout << lines.size() << " unique records read." << endl;
        }
    }

    progress = 0;

    vector<wstring> lineVector(lines.begin(), lines.end());

#pragma omp parallel for
    for (auto i = 0; i < static_cast<int>(lineVector.size()); i++)
    {
#pragma omp atomic
        progress++;
        if (progress % 1000 == 0)
        {
#pragma omp critical
            {
                if (progress % 1000 == 0)
                {
                    wcout << "Parsing record #" << progress << " in thread #" << omp_get_thread_num() << endl;
                }
            }
        }

        const auto firstComma = lineVector[i].find_first_of(L',');
        const auto movieName = lineVector[i].substr(0, firstComma);
        const auto userName = lineVector[i].substr(firstComma + 1, lineVector[i].length() - firstComma);

#pragma omp critical
        {
            movieUserMap.insert(make_pair(movieName, userName));
        }
#pragma omp critical
        {
            userMovieMap.insert(make_pair(userName, movieName));
        }
    }

    progress = 0;

    vector<wstring> movieVector(movies.begin(), movies.end());
#pragma omp parallel for
    for (auto i = 0; i < static_cast<int>(movieVector.size()); i++)
    {
#pragma omp atomic
        progress++;
        if (progress % 10 == 0)
        {
#pragma omp critical
            {
                if (progress % 10 == 0)
                {
                    wcout << "Building movie #" << progress << " in thread #" << omp_get_thread_num() << endl;
                }
            }
        }

        const auto movieName = movieVector[i];
        auto& connectedMovies = movieGraph.Movies.find(movieName)->second->Connections;
        const auto watchedUsers = movieUserMap.equal_range(movieName);
        for (auto userIter = watchedUsers.first; userIter != watchedUsers.second; ++userIter)
        {
            const auto userName = userIter->second;
            const auto movieWatched = userMovieMap.equal_range(userName);
            for (auto movieIter = movieWatched.first; movieIter != movieWatched.second; ++movieIter)
            {
                const auto connectingMovieName = movieIter->second;
                if (movieName == connectingMovieName)
                {
                    continue;
                }

                auto connectingMovie = movieGraph.Movies.find(connectingMovieName)->second;
                const auto result = connectedMovies.find(connectingMovie);
                if (result == connectedMovies.end())
                {
                    connectedMovies.insert(make_pair(connectingMovie, 1));
                }
                else
                {
                    result->second++;
                }
            }
        }
    }

    return movieGraph;
}
