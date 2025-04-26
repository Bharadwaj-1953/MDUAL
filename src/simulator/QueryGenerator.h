#pragma once

#include "../loader/Query.h"
#include <string>
#include <vector>

class QueryGenerator {
public:
    static int numQueries;
    static int defaultW;
    static int gcdS;
    static int defaultK;
    static double defaultR;
    static int nW;
    static int nItr;
    static int variationTimes;
    static std::string dataset;

    static void runMain();
    QueryGenerator(const std::string& data, int defW, int gS, int defK, int varTimes);
    static std::string generate(int numQ, int nW_, const std::vector<std::string>& varyingParams);
    Query generateOne(int qID, const std::vector<std::string>& varyingParams);

private:
    static double randomDouble();
};