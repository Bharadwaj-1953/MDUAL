#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include "../loader/Tuple.h"
#include "../loader/Query.h"
#include "../simulator/QueryGenerator.h"
#include "../simulator/Simulator.hpp"
#include "../simulator/MemoryThread.hpp"

int main() {
    try {
        std::string dataset;
        std::cout << "Enter dataset name (e.g., STK): ";
        std::getline(std::cin, dataset);
        
        int numQueries         = 10;
        double changedQRatio   = 0.2;
        std::vector<std::string> varyingParams = {"R","K","S","W"};
        int defaultW           = 1000;
        int gcdS               = 50;
        int defaultK           = 5;
        int nW                 = 10;
        int repeatNum          = 5;
        int variationTimes     = 10;

        // Create QueryGenerator and generate queries first
        QueryGenerator queryGen(dataset, defaultW, gcdS, defaultK, variationTimes);
        std::string queryset = queryGen.generate(numQueries, nW, varyingParams);
        
        // Print header
        std::printf("%-10s %10s %10s %10s %10s %10s %10s %10s\n",
                    "Dataset", "Queryset", "ChgQRatio", "Time", 
                    "AvgMem", "PeakMem", "#Out", "#OutQ");

        // Create resources
        auto dLoader = std::make_shared<DataLoader>(dataset);
        auto qLoader = std::make_shared<QueryLoader>(queryset);
        auto memThread = std::make_shared<MemoryThread>();

        // Create simulator and run
        Simulator sim(dataset, queryset, dLoader.get(), qLoader.get(), memThread.get());

        for (int i = 0; i < repeatNum; i++) {
            sim.run(nW, numQueries, changedQRatio);
            // Add a short delay between runs
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Exception in testLoad main: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
