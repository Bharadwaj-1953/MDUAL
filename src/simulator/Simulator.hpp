#pragma once

#include "../loader/DataLoader.h"
#include "../loader/QueryLoader.h"
#include "MemoryThread.hpp"
#include <string>

// Forward declarations
class MDUAL;

class Simulator {
private:
    bool verbose; 

public:
    DataLoader* dLoader;
    QueryLoader* qLoader;
    double allTimeSum;
    double allMemSum;
    MemoryThread* memThread;
    std::string dataset;
    std::string queryset;
    int maxW;
    int gcdS;
    int nS;

    Simulator(const std::string& dataset_, const std::string& queryset_,
              DataLoader* dL, QueryLoader* qL, MemoryThread* memThr);
    ~Simulator();
    void run(int nW, int numQueries, double changedQRatio);

    void setVerbose(bool v) { verbose = v; }
};
