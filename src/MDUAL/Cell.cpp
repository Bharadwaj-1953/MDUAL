#include "Cell.h"

Cell::Cell()
    : cellIdx(),
      childCells(),
      tuples(),
      center()
{}

Cell::Cell(const std::vector<short>& cIdx,
           const std::vector<double>& dimLength,
           const std::vector<double>& minValues)
{
    cellIdx = cIdx;
    center.resize(dimLength.size());
    for (size_t j = 0; j < dimLength.size(); j++) {
        center[j] = minValues[j] + cellIdx[j] * dimLength[j]
                    + dimLength[j] / 2.0;
    }
}

Cell::Cell(const std::vector<short>& cIdx,
           const std::vector<double>& cellCenter)
{
    cellIdx = cIdx;
    center  = cellCenter;
}

Cell::Cell(const std::vector<short>& cIdx,
           const std::vector<double>& cellCenter,
           bool subDimFlag)
{
    cellIdx = cIdx;
    center  = cellCenter;
    // childCells is already an empty map.
}

int Cell::getNumTuples() const {
    return static_cast<int>(tuples.size());
}

void Cell::addTupleSubDim(Tuple* t,
                          const std::vector<double>& dimLength,
                          const std::vector<double>& minValues)
{
    tuples.insert(t);
    if (childCells.find(t->fullDimCellIdx) == childCells.end()) {
        childCells[t->fullDimCellIdx] =
            Cell(t->fullDimCellIdx, dimLength, minValues);
    }
    childCells[t->fullDimCellIdx].addTuple(t);
}

void Cell::addTuple(Tuple* t) {
    tuples.insert(t);
}

