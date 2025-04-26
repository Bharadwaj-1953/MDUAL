#pragma once
#include "../loader/Tuple.h"
#include <vector>
#include <cmath>
#include <limits>

class Utils {
public:
    // Core distance computation functions for outlier detection
    
    // Computes exact Euclidean distance between two tuples
    static double distTuple(const Tuple& t1, const Tuple& t2);
    
    // Optimized distance computation that stops if threshold is exceeded
    static double distTuple(const Tuple& t1, const Tuple& t2, double threshold);
    
    static bool isNeighborTuple(const Tuple& t1, const Tuple& t2, double threshold);
    static bool isNeighborTupleCell(const std::vector<double>& v1, 
                                   const std::vector<double>& v2, 
                                   double threshold);
    static double getNeighborCellDist(const std::vector<short>& c1,
                                     const std::vector<short>& c2,
                                     double minR,
                                     double threshold);
    static bool isNeighborCell(const std::vector<short>& c1,
                              const std::vector<short>& c2,
                              double minR,
                              double threshold);
};
