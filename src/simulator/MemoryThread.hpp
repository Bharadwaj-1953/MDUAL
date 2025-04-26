#pragma once

#include <atomic>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <unistd.h> 
#include <sstream>

double getCurrentMemoryUsageMB();

class MemoryThread {
private:
    bool running;
    std::atomic<double> maxMemory;
    std::atomic<double> currentMemory;

public:
    MemoryThread();
    virtual ~MemoryThread() = default;
    
    void start();
    void stop();
    void run();
    void resetMaxMemory(); 
    
    double getMaxMemory() const;
    double getCurrentSnapshot() const;
    void updateMemory();
};

