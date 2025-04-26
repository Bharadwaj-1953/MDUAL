#include "QueryLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <vector>

QueryLoader::QueryLoader(const std::string& queryset) 
    : maxW(std::numeric_limits<int>::lowest()),
      gcdS(std::numeric_limits<int>::max()),
      minR(std::numeric_limits<double>::max())
{
    filePath = "querysets/" + queryset + ".csv";
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("QueryLoader: Unable to open file: " + filePath);
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> rawValues;
        while (std::getline(ss, token, ',')) {
            rawValues.push_back(token);
        }
        if (rawValues.size() < 7) {
            continue;
        }
        double R = std::stod(rawValues[3]);
        int W    = std::stoi(rawValues[5]);
        int S    = std::stoi(rawValues[6]);

        if (W > maxW) maxW = W;
        if (S < gcdS)  gcdS = S;
        if (R < minR)  minR = R;
    }
    fin.close();
}

std::unordered_map<int, Query> QueryLoader::getQuerySet(int curr_itr) {
    std::unordered_map<int, Query> querySet;
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("QueryLoader: Unable to open file: " + filePath);
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> rawValues;
        while (std::getline(ss, token, ',')) {
            rawValues.push_back(token);
        }
        if (rawValues.size() < 7) continue;

        int id     = std::stoi(rawValues[0]);
        int s_time = std::stoi(rawValues[1]);
        int e_time = std::stoi(rawValues[2]);
        if (s_time <= curr_itr && curr_itr < e_time) {
            double R = std::stod(rawValues[3]);
            int K    = std::stoi(rawValues[4]);
            int W    = std::stoi(rawValues[5]);
            int S    = std::stoi(rawValues[6]);

            Query q(id, R, K, W, S);
            querySet[id] = q;
        }
    }
    fin.close();
    return querySet;
}

std::unordered_map<int, Query> QueryLoader::getQuerySetByQID(int fromQID, int numQueries) {
    std::unordered_map<int, Query> querySet;
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("QueryLoader: Unable to open file: " + filePath);
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> rawValues;
        while (std::getline(ss, token, ',')) {
            rawValues.push_back(token);
        }
        if (rawValues.size() < 7) continue;

        int id = std::stoi(rawValues[0]);
        if (id >= fromQID && id < (fromQID + numQueries)) {
            double R = std::stod(rawValues[3]);
            int K    = std::stoi(rawValues[4]);
            int W    = std::stoi(rawValues[5]);
            int S    = std::stoi(rawValues[6]);

            Query q(id, R, K, W, S);
            querySet[id] = q;
        }
    }
    fin.close();
    return querySet;
}
