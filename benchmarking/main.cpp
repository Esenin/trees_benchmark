#include "treeTestLauncher.h"

int main()
{
    benchmark::Benchmaker benchmark;
//    bTreeBinSearchTuning(benchmark);
//    bTreeParamTuning(benchmark);
    treesBenchmarkSummary(benchmark);
//    treesBenchmarkCoverage(benchmark);

    return 0;
}


