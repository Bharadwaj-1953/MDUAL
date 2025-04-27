#pragma once
#include <vector>
#include <unordered_map>
#include "../loader/Query.h"
#include "Cell.h"

// Maintains global statistics and relationships between cells
class globalCell {
public:
    std::vector<short> cellIdx;              // Cell's position in global grid
    std::unordered_map<std::vector<short>, double, VecShortHash, VecShortEq> neighCellMap;  // Maps neighboring cells to their distances
    bool lastUpdated;                        // Track if cell needs updating
    int card;                               // Total number of points in cell
    std::unordered_map<int, int> cardPerSlide;  // Point count per time slide
    std::vector<int> IndirectOutlierCellQueryIDs = {};  // Queries where this cell might contain outliers
    std::vector<Query> ndQueries = {};       // Relevant queries for density-based processing

    globalCell();
    globalCell(const std::vector<short>& cIdx);

    int getCardTotal(int firstSlideID) const;
    std::vector<std::vector<short>> getThredNeighCellsOut(double distThred) const;
    std::vector<std::vector<short>> getThredNeighCellsIn(double distThred) const;
};
