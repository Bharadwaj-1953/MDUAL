#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <iostream>

// Constructor implementation
DataLoader::DataLoader(const std::string& dataset) {
    filePath = "datasets/" + dataset + ".csv";

    // Open the file to read the first line
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("DataLoader: Unable to open file: " + filePath);
    }
    std::string line;
    if (!std::getline(fin, line)) {
        throw std::runtime_error("DataLoader: File is empty: " + filePath);
    }


    {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> rawValues;
        while (std::getline(ss, token, ',')) {
            rawValues.push_back(token);
        }
        dim = static_cast<int>(rawValues.size());
        // If dim > 15, subDim=3, else subDim=dim
        subDim = (dim > 15) ? 3 : dim;
        minValues.resize(dim, std::numeric_limits<double>::max());
        maxValues.resize(dim, std::numeric_limits<double>::lowest());
        priorityList.reserve(dim);
        for (int i = 0; i < dim; i++) {
            priorityList.push_back(i);
        }
        // Process the first line's data
        for (int i = 0; i < dim; i++) {
            double val = std::stod(rawValues[i]);
            if (val < minValues[i]) minValues[i] = val;
            if (val > maxValues[i]) maxValues[i] = val;
        }
    }

    // Now read the rest of the file to update min/max
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string token;
        int i = 0;
        while (std::getline(ss, token, ',')) {
            double val = std::stod(token);
            if (val < minValues[i]) minValues[i] = val;
            if (val > maxValues[i]) maxValues[i] = val;
            i++;
            if (i >= dim) break; 
        }
    }
    fin.close();
}

std::vector<Tuple> DataLoader::getNewSlideTuples(int itr, int S) {
    std::vector<Tuple> newSlide;
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        return newSlide;
    }

    std::string line;
    int tid = 0;
    int startLine = itr * S;
    int endLine   = (itr + 1) * S;

    while (std::getline(fin, line)) {
        if (tid >= startLine && tid < endLine) {
            try {
                
                std::stringstream ss(line);
                std::string token;
                std::vector<double> values(dim);
                std::vector<std::string> rawTokens;
                while (std::getline(ss, token, ',')) {
                    rawTokens.push_back(token);
                }
                for (int i = 0; i < dim; i++) {
                    int col = priorityList[i];
                    if (col >= 0 && col < static_cast<int>(rawTokens.size())) {
                        values[i] = std::stod(rawTokens[col]);
                    }
                }
                newSlide.emplace_back(tid, itr, values);
            } catch (const std::exception& ex) {
                
            }
        }
        tid++;
        if (tid >= endLine) {
            break;
        }
    }

    fin.close();
    return newSlide;
}

const std::vector<double>& DataLoader::getMinValues() const {
    return minValues;
}

const std::vector<double>& DataLoader::getMaxValues() const {
    return maxValues;
}
