#include "timer.h"
#include <Windows.h>
#include <ctime>
#include <fmt/format.h>

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

    const std::string date() {
        time_t now;
        time(&now);

        struct tm* local = localtime(&now);

        int hours, minutes, seconds, day, month, year;
        hours = local->tm_hour;      	// get hours since midnight (0-23)
        minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
        seconds = local->tm_sec;     	// get seconds passed after minute (0-59)

        day = local->tm_mday;        	// get day of month (1 to 31)
        month = local->tm_mon + 1;   	// get month of year (0 to 11)
        year = local->tm_year + 1900;	// get year since 1900
        return fmt::format("{0}{1}{2}{3}{4}{5}", year, month, day, hours, minutes, seconds);
    }
}