#ifndef QUERY_HPP
#define QUERY_HPP

#include <unordered_set>
#include <vector>

class Query {
public:
    int id;
    double R;
    int K;
    int W;
    int S;
    std::unordered_set<int> outliers;
    std::vector<int> layerCnt;

    Query() : id(0), R(0), K(0), W(0), S(0) {}
    
    Query(int id_, double R_, int K_, int W_, int S_)
        : id(id_), R(R_), K(K_), W(W_), S(S_) {}
};

#endif // QUERY_HPP
