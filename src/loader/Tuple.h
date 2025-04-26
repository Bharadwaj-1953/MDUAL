#pragma once
#include <vector>
#include <set>

struct Tuple {
    std::vector<double> value;               // Actual data point coordinates
    std::vector<short> fullDimCellIdx;       // Cell index in full dimensional grid
    std::vector<short> subDimCellIdx;        // Cell index in reduced dimensional grid
    int slideID;
    std::set<int> outlierQueryIDs;
    int id; 
    Tuple() = default;
    Tuple(int id_, int slideID_, const std::vector<double>& value_)
        : value(value_), slideID(slideID_), id(id_) {}
};
