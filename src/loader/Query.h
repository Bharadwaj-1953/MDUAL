#pragma once

#include <unordered_set>
#include <vector>

class Query {
public:
    int id;
    double R;
    int K;
    int W;
    int S;
    std::unordered_set<int> outliers;  // Store tuple IDs of outliers for this query

    Query() : id(0), R(0), K(0), W(0), S(0) {}
    
    Query(int id_, double R_, int K_, int W_, int S_)
        : id(id_), R(R_), K(K_), W(W_), S(S_) {}

    void addOutlier(int tupleId) {
        outliers.insert(tupleId);
    }
};
