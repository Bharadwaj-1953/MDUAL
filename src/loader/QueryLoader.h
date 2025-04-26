#pragma once

#include "Query.h"
#include <string>
#include <unordered_map>

class QueryLoader {
public:
    int maxW;       // Maximum window size across all queries
    int gcdS;       // Greatest common divisor of all slide sizes
    double minR;    // Minimum radius parameter across all queries

private:
    std::string filePath;  // Path to query definition file

public:
    // Loads queries from a CSV file in 'querysets' directory
    QueryLoader(const std::string& queryset);
    
    // Gets active queries for current iteration
    std::unordered_map<int, Query> getQuerySet(int curr_itr);
    
    // Gets a specific range of queries by their IDs
    std::unordered_map<int, Query> getQuerySetByQID(int fromQID, int numQueries);
};
