#include "Utils.h"
#include <cmath>
#include <limits>

double Utils::distTuple(const Tuple& t1, const Tuple& t2) {
    double ss = 0;
    for(size_t i = 0; i < t1.value.size(); i++) { 
        ss += std::pow((t1.value[i] - t2.value[i]), 2);
    }
    return std::sqrt(ss);
}

double Utils::distTuple(const Tuple& t1, const Tuple& t2, double threshold) {
    double ss = 0;
    double ss_thred = threshold * threshold;
    for(size_t i = 0; i < t1.value.size(); i++) { 
        ss += std::pow((t1.value[i] - t2.value[i]), 2);
        if(ss > ss_thred) return std::numeric_limits<double>::max();
    }
    return std::sqrt(ss);
}

bool Utils::isNeighborTuple(const Tuple& t1, const Tuple& t2, double threshold) {
    double ss = 0;
    threshold *= threshold;
    for(size_t i = 0; i < t1.value.size(); i++) { 
        ss += std::pow((t1.value[i] - t2.value[i]), 2);
        if(ss > threshold) return false;
    }
    return true;
}

bool Utils::isNeighborTupleCell(const std::vector<double>& v1, const std::vector<double>& v2, double threshold) {
    double ss = 0;
    threshold *= threshold;
    for(size_t i = 0; i < v2.size(); i++) { 
        ss += std::pow((v1[i] - v2[i]), 2);
        if(ss > threshold) return false;
    }
    return true;
}

double Utils::getNeighborCellDist(const std::vector<short>& c1, const std::vector<short>& c2, double minR, double threshold) {
    double ss = 0;
    for(size_t k = 0; k < c1.size(); k++) {
        ss += std::pow((c1[k] - c2[k]), 2);
        if (ss/c1.size()*minR*minR >= threshold*threshold) 
            return std::numeric_limits<double>::max();
    }
    return std::sqrt(ss/c1.size())*minR;
}

bool Utils::isNeighborCell(const std::vector<short>& c1, const std::vector<short>& c2, double minR, double threshold) {
    double ss = 0;
    for(size_t k = 0; k < c1.size(); k++) {
        ss += std::pow((c1[k] - c2[k]), 2);
        if (ss/c1.size()*minR*minR >= threshold*threshold) return false;
    }
    return true;
}
