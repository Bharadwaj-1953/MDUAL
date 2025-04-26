#include "MemoryThread.hpp"
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>

double getCurrentMemoryUsageMB() {
    double megabyte = 1024.0 * 1024.0;
    #ifdef __APPLE__
        struct rusage rusage;
        getrusage(RUSAGE_SELF, &rusage);
        return static_cast<double>(rusage.ru_maxrss) / megabyte;
    #else
        // Read from /proc/self/status
        std::ifstream status("/proc/self/status");
        std::string line;
        while (std::getline(status, line)) {
            if (line.compare(0, 6, "VmSize") == 0) {
                long kb;
                std::stringstream ss(line);
                std::string tmp;
                ss >> tmp >> kb;
                return static_cast<double>(kb) / 1024.0;
            }
        }
    #endif
    return 0.0;
}

MemoryThread::MemoryThread() : running(false), maxMemory(0.0) {}

void MemoryThread::start() {
    running = true;
    std::thread([this]() { this->run(); }).detach();
}

void MemoryThread::stop() {
    running = false;
}

double MemoryThread::getMaxMemory() const {
    return maxMemory.load();
}

void MemoryThread::resetMaxMemory() {
    maxMemory.store(0.0);
}

void MemoryThread::updateMemory() {
    double used = getCurrentMemoryUsageMB();
    currentMemory.store(used);
    if (used > maxMemory.load()) {
        maxMemory.store(used);
    }
}

double MemoryThread::getCurrentSnapshot() const {
    return currentMemory.load();
}

void MemoryThread::run() {
    while (running) {
        updateMemory();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
