#include <string>

#define X

#ifdef GOOGLE_BENCHMARK
#include "../benchmark/include/benchmark/benchmark_api.h"


static void BM_StringCreation(benchmark::State& state) {
    while (state.KeepRunning())
        std::string empty_string;
}

// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    while (state.KeepRunning())
        std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();

#else

#include "treeTestLauncher.h"

int main()
{
    benchmark::Benchmaker benchmark;
    treesBenchmark(benchmark);

    return 0;
}
#endif

