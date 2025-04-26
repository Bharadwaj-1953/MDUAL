#include "Simulator.hpp"
#include "../MDUAL/MDUAL.h"
#include "../loader/DataLoader.h"
#include "../loader/QueryLoader.h"
#include "Measure.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

// Helper function to convert microseconds to milliseconds for output
double formatTimeOutput(double originalTime) {
    return (originalTime / 1000.0); 
}

// Constructor implementation
Simulator::Simulator(const std::string& dataset_, const std::string& queryset_, 
                    DataLoader* dL, QueryLoader* qL, MemoryThread* memThr)
    : dLoader(dL), qLoader(qL), dataset(dataset_), queryset(queryset_),
      allTimeSum(0.0), allMemSum(0.0), memThread(memThr),
      gcdS(qL->gcdS), nS(qL->maxW / qL->gcdS)
{

}

// Destructor implementation
Simulator::~Simulator() {
    // Any cleanup needed
}

// Run method implementation
void Simulator::run(int nW, int numQueries, double changedQRatio) {
    try {
        // Create MDUAL instance with dimension reduction parameters
        MDUAL md(dLoader->dim, dLoader->subDim, nS, gcdS, dLoader->getMinValues());
        
        // Start memory monitoring thread
        memThread->start();

        // Track statistics for performance evaluation
        int numWin = 0;
        int numChangedQueries = static_cast<int>(numQueries * changedQRatio);
        int totalOutliers = 0;
        int totalOutQueries = 0;
        
        for (int itr = 0; itr < nW + nS - 1; itr++) {
            auto newQuerySet = qLoader->getQuerySetByQID(itr * numChangedQueries, numQueries);
            if (newQuerySet.empty()) break;
            
            auto newSlideTuples = dLoader->getNewSlideTuples(itr, gcdS);
            if (newSlideTuples.empty()) break;
            
            long startTime = Measure::getCPUTime();
            auto outliers = md.findOutlier(newSlideTuples, newQuerySet, itr);
            long endTime = Measure::getCPUTime();
            
            if (itr >= nS - 1) {
                double elapsedMs = (endTime - startTime) / 1000000.0;
                allTimeSum += elapsedMs;
                memThread->updateMemory();
                allMemSum += memThread->getCurrentSnapshot();
                
                totalOutliers += outliers.size();
                for (auto* tuple : outliers) {
                    totalOutQueries += tuple->outlierQueryIDs.size();
                }
                numWin++;
            }
        }

        if (numWin > 0) {
            double avgTime = allTimeSum / numWin;
            double avgMem = allMemSum / numWin;
            double avgOutliers = static_cast<double>(totalOutliers) / numWin;
            double avgOutQueries = static_cast<double>(totalOutQueries) / numWin;

            printf("%-10s %10s %10.1f %10.2f %10.1f %10.1f %10d %10d\n",
                   dataset.c_str(),
                   queryset.c_str(),
                   changedQRatio,
                   formatTimeOutput(avgTime),
                   avgMem,
                   memThread->getMaxMemory(),
                   static_cast<int>(avgOutliers),
                   static_cast<int>(avgOutQueries));
        }

        memThread->stop();

    } catch (const std::exception& ex) {
        std::cerr << "Exception in Simulator::run: " << ex.what() << std::endl;
    }
}
