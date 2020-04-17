#include "timer.h"
#include <Windows.h>

namespace lxd {
    void sleep(int milliseconds) {
        Sleep(milliseconds);
    }

    uint64_t nanosecond() {
        static uint64_t ticksPerSecond = 0;
        static uint64_t timeBase = 0;

        if(ticksPerSecond == 0) {
            LARGE_INTEGER li;
            QueryPerformanceFrequency(&li);
            ticksPerSecond = (uint64_t)li.QuadPart;
            QueryPerformanceCounter(&li);
            timeBase = (uint64_t)li.LowPart + 0xFFFFFFFFULL * li.HighPart;
        }

        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        uint64_t counter = (uint64_t)li.LowPart + 0xFFFFFFFFULL * li.HighPart;
        return (counter - timeBase) * 1000ULL * 1000ULL * 1000ULL / ticksPerSecond;
	}

    float millisecond() {
        return float(nanosecond()) * 0.000001f;
    }

    double second() {
        return double(nanosecond()) * 0.000000001;
    }
}