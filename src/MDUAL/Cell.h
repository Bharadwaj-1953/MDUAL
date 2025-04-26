#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cstddef>
#include "../loader/Tuple.h"

// Hash function for vector<short> to enable use as key in unordered_map
struct VecShortHash {
    std::size_t operator()(const std::vector<short>& v) const {
        std::size_t h = 0;
        for (auto &elem : v) {
            h ^= std::hash<short>()(elem) + 0x9e3779b97f4a7c15ULL
                 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

// Equality comparison for vector<short> to enable use as key in unordered_map
struct VecShortEq {
    bool operator()(const std::vector<short>& a,
                    const std::vector<short>& b) const {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }
};

// Represents a cell in the grid-based partitioning scheme
class Cell {
public:
    std::vector<short> cellIdx;              // Cell's position in the grid
    std::unordered_map<std::vector<short>, Cell, VecShortHash, VecShortEq> childCells;  // For hierarchical grid structure
    std::unordered_set<Tuple*> tuples;       // Points contained in this cell
    std::vector<double> center;              // Geometric center of the cell

    Cell();
    Cell(const std::vector<short>& cIdx,
         const std::vector<double>& dimLength,
         const std::vector<double>& minValues);
    Cell(const std::vector<short>& cIdx,
         const std::vector<double>& cellCenter);
    Cell(const std::vector<short>& cIdx,
         const std::vector<double>& cellCenter,
         bool subDimFlag);


    Cell(const std::vector<short>& cIdx) : cellIdx(cIdx), childCells(), tuples(), center() {}

    int getNumTuples() const;
    void addTupleSubDim(Tuple* t,
                        const std::vector<double>& dimLength,
                        const std::vector<double>& minValues);
    void addTuple(Tuple* t);
};
