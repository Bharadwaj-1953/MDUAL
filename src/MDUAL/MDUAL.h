#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <set>
#include <algorithm>
#include <iterator>
#include <limits>
#include <cmath>
#include <cfloat>
#include <functional>
#include "Cell.h"
#include "globalCell.h"
#include "../loader/Query.h"
#include "../loader/Tuple.h"

inline bool inlierFirstCompare(const Query& q1, const Query& q2);

class MDUAL {
public:
    // Core parameters for outlier detection
    double minR, maxR, minR_old, maxR_old;  // Radius parameters for distance calculations
    int gcdS;                               // Greatest common divisor of slide sizes
    int dim, subDim;                        // Full and reduced dimensionality
    bool subDimFlag;                        // Whether using dimension reduction
    int nS, nW;                             // Number of slides and windows
    std::vector<double> minValues;
    std::vector<double> dimLength, subDimLength;
    std::unordered_map<std::vector<short>, Cell, VecShortHash, VecShortEq> slideIn;
    std::unordered_map<std::vector<short>, Cell, VecShortHash, VecShortEq> slideOut;
    std::unordered_map<std::vector<short>, int, VecShortHash, VecShortEq> slideDeltaCnt;
    std::unordered_map<std::vector<short>, globalCell, VecShortHash, VecShortEq> cardGrid;
    std::unordered_map<std::vector<short>, globalCell, VecShortHash, VecShortEq> fullDimCardGrid;
    std::list<std::unordered_map<std::vector<short>, Cell, VecShortHash, VecShortEq>> slides;
    std::list<std::unordered_map<std::vector<short>, int, VecShortHash, VecShortEq>> fullDimCellSlidesCnt;
    std::unordered_set<Tuple*> outliers;
    std::unordered_map<int, Query> querySet;
    bool maxRChanged, minRChanged;

    MDUAL(int dim_, int subDim_, int nS_, int gcdS_, const std::vector<double>& minValuesIn);

    ~MDUAL();

    std::unordered_set<Tuple*>& findOutlier(std::vector<Tuple>& newSlideTuples,
                                            const std::unordered_map<int, Query>& newQuerySet,
                                            int itr);
    void initCellSize();
    void clearPreviousOutliers();
    void updateBasisParams(int itr);
    void updateWindow(const std::vector<Tuple>& slideTuples, int itr);
    void findOutlierMain(int itr);
    void getNeighCellMap(const std::unordered_set<std::vector<short>, VecShortHash, VecShortEq> &newCellIndices);
    void reComputeNeighCellMap();
    void reIndexCardGrid(int itr);

    bool validateDimensions() const {
        return !minValues.empty() && 
               minValues.size() >= dim && 
               dimLength.size() >= dim && 
               subDimLength.size() >= subDim;
    }

private:
    bool isLikelyInlier(Tuple* tuple, const Query& q) const;
};
