#pragma once

class Measure {
public:
    static long peakUsedMemory;

private:
    static constexpr int MegaBytes = 1024 * 1024;
    Measure() = default;

public:
    static long getCPUTime();
    static long getMemory();
};
