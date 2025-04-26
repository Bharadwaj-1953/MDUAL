#pragma once
#include <string>
#include <vector>
#include "Tuple.h"

class DataLoader {
private:
    // Stores the minimum and maximum values for each dimension in the dataset
    std::vector<double> minValues;
    std::vector<double> maxValues;
    std::string filePath;
    std::vector<int> priorityList;  // Defines dimension priority for processing

public:
    int dim;      // Total number of dimensions in the dataset
    int subDim;   // Number of dimensions used in reduced space (for efficiency)

    DataLoader(const std::string& dataset);
    std::vector<Tuple> getNewSlideTuples(int itr, int S);
    const std::vector<double>& getMinValues() const;
    const std::vector<double>& getMaxValues() const;
};
