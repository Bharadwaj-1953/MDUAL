#include "globalCell.h"

globalCell::globalCell() : lastUpdated(false), card(0) {}

globalCell::globalCell(const std::vector<short>& cIdx)
    : cellIdx(cIdx),
      lastUpdated(false),
      card(0)
{

}

int globalCell::getCardTotal(int firstSlideID) const {
    int total = 0;
    for (auto &kv : cardPerSlide) {
        int slideID = kv.first;
        int c = kv.second;
        if (slideID >= firstSlideID) {
            total += c;
        }
    }
    return total;
}

std::vector<std::vector<short>> globalCell::getThredNeighCellsOut(double distThred) const {
    std::vector<std::vector<short>> thredNeighCells;
    thredNeighCells.reserve(neighCellMap.size());
    for (auto &kv : neighCellMap) {
        if (kv.second < distThred) {
            thredNeighCells.push_back(kv.first);
        }
    }
    return thredNeighCells;
}

std::vector<std::vector<short>> globalCell::getThredNeighCellsIn(double distThred) const {
    std::vector<std::vector<short>> thredNeighCells;
    thredNeighCells.reserve(neighCellMap.size());
    for (auto &kv : neighCellMap) {
        if (kv.second <= distThred) {
            thredNeighCells.push_back(kv.first);
        }
    }
    return thredNeighCells;
}
