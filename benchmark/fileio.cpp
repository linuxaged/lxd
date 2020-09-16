#include <benchmark/benchmark.h>
#include "../src/numbers.h"
#include <charconv>

static void BM_SimpleAtof(benchmark::State& state) {
    for(auto _ : state) {
        float pi{};
        lxd::SimpleAtof("3.1415926", &pi);
    }
}
BENCHMARK(BM_SimpleAtof);

static void BM_fromChars(benchmark::State& state) {
    const char* str = "3.1415926";
    auto size = strlen(str) - 1;
    for(auto _ : state) {
        float pi{};
        std::from_chars(str, str + size, pi);
    }
}
BENCHMARK(BM_fromChars);

BENCHMARK_MAIN();