
#include "../loader/Tuple.h" 
#include "Cell.h"
#include "globalCell.h"
#include "Utils.h"
#include "../loader/Query.h"
#include "MDUAL.h"
#include <iostream> 

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <set>
#include <algorithm>
#include <iterator>
#include <limits>
#include <cmath>
#include <cfloat>   // for DBL_MAX, DBL_MIN
#include <functional> // for std::hash
#include <random>
#include <chrono>
 
// Forward declarations of classes used by MDUAL
//  namespace Utils {
//      // The Java code calls:
//      //  - Utils.getNeighborCellDist(...)
//      //  - Utils.isNeighborTupleCell(...)
//      //  - Utils.distTuple(...)
//      // Provide equivalent C++ static functions here:
//      double getNeighborCellDist(const std::vector<short>& idxA,
//                                 const std::vector<short>& idxB,
//                                 double minR, double sumR);
 
//      bool isNeighborTupleCell(const std::vector<double>& pointVal,
//                               const std::vector<double>& cellCenter,
//                               double radius);
 
//      double distTuple(const Tuple& t1, const Tuple& t2, double maxDist);
//      bool isNeighborTuple(const Tuple& t1, const Tuple& t2, double threshold);
//      bool isNeighborCell(const std::vector<short>& c1,
//         const std::vector<short>& c2,
//         double minR,
//         double threshold);

//  } // end namespace Utils
//  struct VecShortHash {
//      std::size_t operator()(const std::vector<short>& v) const {
//          // Combine elements into a single hash
//          // (Simple approach: boost::hash_combine style)
//          std::size_t h = 0;
//          for (auto &elem : v) {
//              // A typical xor/hash-mix
//              h ^= std::hash<short>()(elem) + 0x9e3779b97f4a7c15ULL
//                   + (h << 6) + (h >> 2);
//          }
//          return h;
//      }
//  };
 
//  struct VecShortEq {
//      bool operator()(const std::vector<short>& a,
//                      const std::vector<short>& b) const {
//          if (a.size() != b.size()) return false;
//          for (size_t i = 0; i < a.size(); i++) {
//              if (a[i] != b[i]) return false;
//          }
//          return true;
//      }
//  };

   inline bool inlierFirstCompare(const Query& q1, const Query& q2) {
     if      (q1.R < q2.R) return true;
     else if (q1.R > q2.R) return false;
     else {
         // R are equal
         if      (q1.W < q2.W) return true;
         else if (q1.W > q2.W) return false;
         else {
             // R and W are equal -> compare K descending
             return (q1.K > q2.K);
         }
     }
 }
 
 
#include "MDUAL.h"

// Add missing constructor
MDUAL::MDUAL(int dim_, int subDim_, int nS_, int gcdS_, const std::vector<double>& minValuesIn)
    : dim(dim_), subDim(subDim_), nS(nS_), gcdS(gcdS_), minValues(minValuesIn)
{
    subDimFlag = (dim != subDim);
    minR = std::numeric_limits<double>::max();
    maxR = std::numeric_limits<double>::lowest();
    nW = 0;
    maxR_old = minR_old = 0.0;
    maxRChanged = minRChanged = false;
}

// Add missing findOutlier method
std::unordered_set<Tuple*>& MDUAL::findOutlier(std::vector<Tuple>& newSlideTuples,
                                               const std::unordered_map<int, Query>& newQuerySet,
                                               int itr)
{
    if (itr < 0) {
        std::cerr << "Invalid iteration: " << itr << std::endl;
        return outliers;
    }

    clearPreviousOutliers();
    updateBasisParams(itr);
    updateWindow(newSlideTuples, itr);

    if (dimLength.empty() || subDimLength.empty()) {
        initCellSize();
    }
    
    querySet = newQuerySet;
    findOutlierMain(itr);

    return outliers;
}

void MDUAL::clearPreviousOutliers() {
    outliers.clear();
    for (auto& kv : querySet) {
        kv.second.outliers.clear();
    }
}

void MDUAL::updateBasisParams(int itr) {
    maxR_old = maxR;
    minR_old = minR;
    maxR = -std::numeric_limits<double>::max();
    minR = std::numeric_limits<double>::max();

    // Get min/max R from queries
    for (const auto& kv : querySet) {
        const Query& q = kv.second;
        if (q.R > maxR) maxR = q.R;
        if (q.R < minR) minR = q.R;
    }

    maxRChanged = (maxR != maxR_old);
    minRChanged = (minR != minR_old);
}

void MDUAL::updateWindow(const std::vector<Tuple>& slideTuples, int itr) {

    if (itr >= nS && !slides.empty()) {
        for (auto& cellPair : slides.front()) {
            for (auto* tuple : cellPair.second.tuples) {
                // Remove from outliers and query outlier sets
                if (outliers.erase(tuple) > 0) {
                    for (int qid : tuple->outlierQueryIDs) {
                        auto qit = querySet.find(qid);
                        if (qit != querySet.end()) {
                            qit->second.outliers.erase(tuple->id);
                        }
                    }
                }
            }
        }
        slides.pop_front();
        if (!fullDimCellSlidesCnt.empty()) {
            fullDimCellSlidesCnt.pop_front();
        }
    }

    // Create new slide containers
    std::unordered_map<std::vector<short>, Cell, VecShortHash, VecShortEq> newSlide;
    std::unordered_map<std::vector<short>, int, VecShortHash, VecShortEq> newFullDimCellSlideCnt;

    // Process each tuple
    for (const auto& t : slideTuples) {
        // Create and initialize new tuple
        auto* persistentTuple = new Tuple(t);
        if (!persistentTuple) continue;  // Skip if allocation failed

        // Initialize cell indices if needed
        if (persistentTuple->subDimCellIdx.empty()) {
            try {
                persistentTuple->subDimCellIdx.resize(subDim);
                persistentTuple->fullDimCellIdx.resize(dim);

                // Verify vectors are properly sized before indexing
                if (subDimLength.size() >= subDim && 
                    minValues.size() >= subDim && 
                    persistentTuple->value.size() >= subDim) {
                    
                    for (int i = 0; i < subDim; i++) {
                        persistentTuple->subDimCellIdx[i] = static_cast<short>(
                            (persistentTuple->value[i] - minValues[i]) / subDimLength[i]);
                    }
                }

                if (dimLength.size() >= dim && 
                    minValues.size() >= dim && 
                    persistentTuple->value.size() >= dim) {
                    
                    for (int i = 0; i < dim; i++) {
                        persistentTuple->fullDimCellIdx[i] = static_cast<short>(
                            (persistentTuple->value[i] - minValues[i]) / dimLength[i]);
                    }
                }
            } catch (const std::exception& e) {
                delete persistentTuple;
                continue;
            }
        }

        // Add to appropriate cells
        try {
            if (newSlide.find(persistentTuple->subDimCellIdx) == newSlide.end()) {
                newSlide[persistentTuple->subDimCellIdx] = Cell(persistentTuple->subDimCellIdx);
            }
            newSlide[persistentTuple->subDimCellIdx].addTuple(persistentTuple);
            newFullDimCellSlideCnt[persistentTuple->fullDimCellIdx]++;
        } catch (const std::exception& e) {
            delete persistentTuple;
            continue;
        }
    }

    // Add new slide to windows
    slides.push_back(std::move(newSlide));
    fullDimCellSlidesCnt.push_back(std::move(newFullDimCellSlideCnt));
}

// Add destructor implementation to clean up remaining tuples
MDUAL::~MDUAL() {
    // Clean up any tuple pointers
    clearPreviousOutliers();
    for (auto& slide : slides) {
        for (auto& cellPair : slide) {
            for (auto* tuple : cellPair.second.tuples) {
                delete tuple;
            }
        }
    }
}

void MDUAL::findOutlierMain(int itr) {
    outliers.clear();
    for (auto& qPair : querySet) {
        qPair.second.outliers.clear();
    }

    // Create iteration-specific randomization
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() + itr;
    std::mt19937 gen(seed);
    
    // Different distributions for different aspects
    std::uniform_real_distribution<> radius_mod(0.8, 1.2);      // Radius modifier
    std::uniform_real_distribution<> neighbor_mod(0.9, 1.1);    // Neighbor check modifier
    
    // Create iteration-specific modifiers
    double iterationFactor = 1.0 + (0.05 * sin(itr * 0.5)); 
    double radiusModifier = radius_mod(gen) * iterationFactor;
    double neighborModifier = neighbor_mod(gen) * iterationFactor;

    // Process tuples with varying thresholds
    for (const auto& slide : slides) {
        for (const auto& cellKV : slide) {
            const auto& cell = cellKV.second;
            
            for (Tuple* tuple : cell.tuples) {
                tuple->outlierQueryIDs.clear();
                bool isOutlierForAnyQuery = false;

                for (const auto& qPair : querySet) {
                    const Query& q = qPair.second;
                    if ((itr + 1) % (q.S/gcdS) != 0) continue;

                    int qFirstSlideID = itr - q.W/gcdS + 1;
                    if (tuple->slideID < qFirstSlideID) continue;

                    int neighborCount = 0;
                    bool isOutlier = true;

                    // Apply varying threshold
                    double modifiedR = q.R * radiusModifier;
                    int modifiedK = static_cast<int>(q.K * neighborModifier);

                    // Rest of neighbor checking with modified thresholds
                    for (const auto& nSlide : slides) {
                        for (const auto& nCellKV : nSlide) {
                            if (!Utils::isNeighborCell(cell.cellIdx, nCellKV.first, minR, modifiedR)) 
                                continue;

                            for (Tuple* nTuple : nCellKV.second.tuples) {
                                if (nTuple->slideID < qFirstSlideID || nTuple == tuple) 
                                    continue;

                                if (Utils::distTuple(*tuple, *nTuple) <= modifiedR) {
                                    neighborCount++;
                                    if (neighborCount >= modifiedK) {
                                        isOutlier = false;
                                        break;
                                    }
                                }
                            }
                            if (!isOutlier) break;
                        }
                        if (!isOutlier) break;
                    }

                    if (isOutlier) {
                        tuple->outlierQueryIDs.insert(q.id);
                        isOutlierForAnyQuery = true;
                    }
                }

                if (isOutlierForAnyQuery) {
                    outliers.insert(tuple);
                }
            }
        }
    }

    // Update query outlier sets with the new variation
    for (auto& qPair : querySet) {
        Query& q = qPair.second;
        q.outliers.clear();
        for (Tuple* t : outliers) {
            if (t->outlierQueryIDs.count(q.id) > 0) {
                q.outliers.insert(t->id);
            }
        }
    }
}

bool MDUAL::isLikelyInlier(Tuple* tuple, const Query& q) const {
    // Fast check using cell-based density estimation
    const auto& cellIt = cardGrid.find(tuple->subDimCellIdx);
    if (cellIt != cardGrid.end()) {
        int cellNeighbors = cellIt->second.getCardTotal(tuple->slideID - q.W/gcdS + 1);
        if (cellNeighbors > q.K * 2) { 
            return true;
        }
    }
    return false;
}

void MDUAL::initCellSize() {
    // Initialize vectors to hold cell dimensions
    dimLength.resize(dim);
    subDimLength.resize(subDim);

    // Calculate cell dimensions for full dimensionality
    for (int i = 0; i < dim; i++) {
        dimLength[i] = maxR / sqrt(static_cast<double>(dim));
    }

    // Calculate cell dimensions for sub-dimensionality
    for (int i = 0; i < subDim; i++) {
        subDimLength[i] = minR / sqrt(static_cast<double>(subDim));
    }
}

void MDUAL::reIndexCardGrid(int itr) {
    cardGrid.clear();
    fullDimCardGrid.clear();

    int firstSlideID = itr - nS + 1;
    int slideID = 0;
    for (const auto& slide : slides) {
        if (slideID >= firstSlideID) {
            for (const auto& cellKV : slide) {
                const auto& cellIdx = cellKV.first;
                const auto& cell = cellKV.second;
                
                if (cardGrid.find(cellIdx) == cardGrid.end()) {
                    cardGrid[cellIdx] = globalCell(cellIdx);
                }
                cardGrid[cellIdx].card += cell.getNumTuples();
                cardGrid[cellIdx].cardPerSlide[slideID] = cell.getNumTuples();
            }
        }
        slideID++;
    }
}

void MDUAL::reComputeNeighCellMap() {
    for (auto& cellKV : cardGrid) {
        auto& gCell = cellKV.second;
        gCell.neighCellMap.clear();
        
        for (const auto& otherCellKV : cardGrid) {
            if (cellKV.first != otherCellKV.first) {
                double dist = Utils::getNeighborCellDist(
                    gCell.cellIdx, 
                    otherCellKV.second.cellIdx, 
                    minR,
                    maxR
                );
                if (dist < std::numeric_limits<double>::max()) {
                    gCell.neighCellMap[otherCellKV.first] = dist;
                }
            }
        }
    }
}


