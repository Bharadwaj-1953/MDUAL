#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include "../loader/Query.h"
#include "../loader/Tuple.h"

// Forward declarations of Query and Tuple, adjust includes as needed.
//  class Query {
//  public:
//      int id;
//      double R;
//      int K;
//      int W;
//      int S;
//      // Possibly more fields...
//  };

//  class Tuple {
//  public:
//      int id;
//      int slideID;
//      std::vector<double> value;  // corresponds to t1.value[]
//      std::unordered_set<int> outlierQueryIDs; // store query IDs for which it's an outlier
//      // ... other fields if needed
//  };

class NAIVE {
public:
    // Core data structures for outlier detection
    std::unordered_map<int, Query> querySet;  // Active queries in current window
    int nS;      // Number of slides in window
    int gcdS;    // Greatest common divisor of slide sizes
    std::vector<Tuple> allTuples;  // All tuples in current window

    // Constructor
    NAIVE(int nS_, int gcdS_)
        : nS(nS_), gcdS(gcdS_)
    {
        // allTuples starts empty
    }

    std::unordered_set<Tuple*> findOutlier(const std::vector<Tuple>& newSlideTuples,
                                          const std::unordered_map<int, Query>& newQuerySet,
                                          int itr) {
        // Sliding window maintenance - remove old tuples
        int firstSlideID = itr - nS + 1;
        if (itr > nS) {
            auto endIt = std::remove_if(allTuples.begin(), allTuples.end(),
                                        [firstSlideID](const Tuple& t) {
                                            return (t.slideID < firstSlideID);
                                        });
            allTuples.erase(endIt, allTuples.end());
        }

        // add newSlideTuples to allTuples
        for (auto &t : newSlideTuples) {
            allTuples.push_back(t);
        }

        // store the new query set
        querySet = newQuerySet;
        std::unordered_set<Tuple*> outliers;
        // For each candTuple in allTuples
        for (Tuple& candTuple : allTuples) {
            // clear its outlierQueryIDs from any previous usage

            candTuple.outlierQueryIDs.clear();

            bool outlierFlag = false;

            // QueryLoop
            for (auto &kv : querySet) {
                const Query &q = kv.second;

                // if((itr+1) % (q.S/gcdS) > 0) continue;
                if (((itr + 1) % (q.S / gcdS)) != 0) {
                    continue;
                }

                // firstSlideID = itr - q.W/gcdS + 1;
                int qFirstSlideID = itr - (q.W / gcdS) + 1;
                if (candTuple.slideID < qFirstSlideID) {
                    // skip if candTuple is outside the query's time window
                    continue;
                }

                // count neighbors within distance q.R
                int nn = 0;
                for (auto &otherTuple : allTuples) {
                    if (otherTuple.slideID < qFirstSlideID) {
                        continue;
                    }
                    double dist = distTuple(candTuple, otherTuple);
                    if ((candTuple.id != otherTuple.id) && (dist <= q.R)) {
                        nn++;
                    }
                }

                if (nn < q.K) {
                    outlierFlag = true;
                    candTuple.outlierQueryIDs.insert(q.id);
                }
            }

            if (outlierFlag) {
                outliers.insert(&candTuple);
            }
        }

        return outliers;
    }

    // Equivalent to: public static double distTuple(Tuple t1, Tuple t2)
    static double distTuple(const Tuple& t1, const Tuple& t2) {
        double ss = 0.0;
        // We'll assume t1.value.size() == t2.value.size()
        for (size_t i = 0; i < t1.value.size(); i++) {
            double diff = t1.value[i] - t2.value[i];
            ss += diff * diff;
        }
        return std::sqrt(ss);
    }
};

