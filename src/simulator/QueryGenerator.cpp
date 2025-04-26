#include "QueryGenerator.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stdexcept>


std::string QueryGenerator::dataset = "";
int QueryGenerator::numQueries = 0;
int QueryGenerator::defaultW = 0;
int QueryGenerator::gcdS = 0;
int QueryGenerator::defaultK = 0;
double QueryGenerator::defaultR = 0.0;
int QueryGenerator::nW = 0;
int QueryGenerator::nItr = 0;
int QueryGenerator::variationTimes = 0;

void QueryGenerator::runMain() {

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    defaultR = 0.5;
    numQueries = 100;
    defaultW = 10000;
    gcdS = 500;
    defaultK = 50;
    nW = 10;
    nItr = defaultW / gcdS + nW;
    variationTimes = 5;
    dataset = "";  

    std::string queryset = generate(numQueries, nW, { "R","K","S","W" });
    std::string filePath = "querysets/" + queryset + ".csv";

    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::cout << line << std::endl;
    }
    fin.close();
}

QueryGenerator::QueryGenerator(const std::string& data,
               int defW,
               int gS,
               int defK,
               int varTimes) 
{
    QueryGenerator::dataset = data;
    QueryGenerator::defaultW = defW;
    QueryGenerator::gcdS = gS;
    QueryGenerator::defaultK = defK;
    QueryGenerator::variationTimes = varTimes;


    if (data == "STK") {
        QueryGenerator::defaultR = 0.5;
    } else if (data == "TAO") {
        QueryGenerator::defaultR = 1.5;
    } else if (data == "HPC") {
        QueryGenerator::defaultR = 10.0;
    } else if (data == "GAS") {
        QueryGenerator::defaultR = 1.5;
    } else if (data == "EM") {
        QueryGenerator::defaultR = 115.0;
    } else if (data == "FC") {
        QueryGenerator::defaultR = 525.0;
    }
}

std::string QueryGenerator::generate(int numQ, int nW_, const std::vector<std::string>& varyingParams) {
    // e.g. "STK_Q100"
    std::string queryset = QueryGenerator::dataset + "_Q" + std::to_string(numQ);
    std::string filePath = "querysets/" + queryset + ".csv";


    {
        std::ifstream test(filePath.c_str());
        if (test.good()) {
            // remove the file
            test.close();
            std::remove(filePath.c_str());
        }
    }

    // Open for writing
    std::ofstream fout(filePath.c_str());
    if (!fout.is_open()) {
        throw std::runtime_error("QueryGenerator: Cannot open file for writing: " + filePath);
    }

    QueryGenerator::nW = nW_;
    QueryGenerator::nItr = defaultW * variationTimes / gcdS + nW_;

    double R = defaultR;
    int K = defaultK;
    int S = gcdS;
    int W = defaultW;

    fout << "0,0," << nItr << "," << defaultR << "," << defaultK << "," << defaultW << "," << gcdS << "\n";


    int limit = numQ * (nItr);
    for (int i = 1; i < limit; i++) {
        int startTime = 0;
        int endTime   = nItr;
        // Reset them to defaults
        R = defaultR;
        K = defaultK;
        S = gcdS;
        W = defaultW;

        // For each param in varyingParams, randomize
        for (auto &param : varyingParams) {
            if (param == "R") {
                double rnd  = (1.0 + randomDouble() * (variationTimes - 1));
                double temp = rnd * defaultR * 100.0;
                temp        = std::round(temp);
                R           = temp / 100.0;
            } else if (param == "K") {
                int rnd  = static_cast<int>(1 + std::floor(randomDouble() * variationTimes));
                K        = defaultK * rnd;
            } else if (param == "S") {
                // S = gcdS*(int) (1 + Math.random()*variationTimes)
                int rnd  = static_cast<int>(1 + std::floor(randomDouble() * variationTimes));
                S        = gcdS * rnd;
            } else if (param == "W") {
                // W = S + S*(int) (Math.random()*((defaultW*variationTimes)/S))
                double maxFactor = (defaultW * variationTimes) / static_cast<double>(S);
                int rnd  = static_cast<int>(std::floor(randomDouble() * maxFactor));
                W        = S + S * rnd;
            }
        }
        fout << i << "," << startTime << "," << endTime << ","
             << R << "," << K << "," << W << "," << S << "\n";
    }

    fout.close();
    return queryset;
}

Query QueryGenerator::generateOne(int qID, const std::vector<std::string>& varyingParams) {
    double R = defaultR;
    int K    = defaultK;
    int S    = gcdS;
    int W    = defaultW;
    for (auto &param : varyingParams) {
        if (param == "R") {
            // R = Math.round((1+Math.random())*defaultR*100)/100d
            double rnd = (1.0 + randomDouble()) * defaultR * 100.0;
            rnd = std::round(rnd);
            R   = rnd / 100.0;
        } else if (param == "K") {
            // K = (int)(1+Math.random()*defaultK*2)
            int rnd = static_cast<int>(1 + std::floor(randomDouble() * (defaultK * 2)));
            K       = rnd;
        } else if (param == "S") {
            // S = gcdS*(int) (1+Math.random()*4)
            int rnd = static_cast<int>(1 + std::floor(randomDouble() * 4));
            S       = gcdS * rnd;
        } else if (param == "W") {
            // W = S+S*(int) (Math.random()*(defaultW/S))
            double maxFactor = (defaultW / static_cast<double>(S));
            int rnd = static_cast<int>(std::floor(randomDouble() * maxFactor));
            W       = S + S * rnd;
        }
    }
    Query q(qID, R, K, W, S);
    return q;
}

double QueryGenerator::randomDouble() {
    return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
}
