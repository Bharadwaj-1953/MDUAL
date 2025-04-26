#include "Measure.h"
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>

// Define the static member
long Measure::peakUsedMemory = 0;

long Measure::getCPUTime() {
    struct timespec ts;
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0) {
        return (ts.tv_sec * 1000000000L) + ts.tv_nsec; 
    }
    return 0;
}

long Measure::getMemory() {
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    return (rusage.ru_maxrss * 1024L); 
}