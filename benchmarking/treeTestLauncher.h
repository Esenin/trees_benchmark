#pragma once

#include <set>
#include <assert.h>
#include <string>
#include <iostream>

#include "../AnyBenchmark/TestingFramework/include/benchmaker.h"
#include "testTreeAdapter.h"

using tree::TestTreeAdapter;
using namespace benchmark;


//! that is a benchmark config
void makeBenchmarkSet(Benchmaker &benchmark, TestTreeAdapter *tree, std::string const &name = "")
{
	benchmark.setRunnableObject(tree);
	benchmark.setBenchmarkName(name);
	benchmark.makeBenchmark();
}

//! Complect of trees for performance test
void treesBenchmark(Benchmaker &benchmark)
{
    int const repeatsNumber = 10;
    double const coverageRatio = 20.0 / 100.0;
    int const startSize = 500 * 1000;
    int const maxSize = 40 * 1000 * 1000;
    int const step = 500 * 1000;

    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setLogginToFile(FileOutput::csv);
    benchmark.setRoundsCount(repeatsNumber);
    benchmark.setDivisionFactor(TestTreeAdapter::lookupCount);
    benchmark.setTestingParam(startSize, maxSize, step);

    benchmark.beginGroup();


    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::avlTree, coverageRatio), "Avl_til40M");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::vebLayoutTree, coverageRatio), "vebLayoutTree");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::splay, coverageRatio), "splay");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(64, coverageRatio), "btree64");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::advancedAvlTree, coverageRatio), "advancedAvlTree");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::stdRBTree, coverageRatio), "stlRBTree");

    benchmark.endGroup();
}

void bTreeParamTuning(Benchmaker &benchmark)
{
    int const repeatsNumber = 6;
    double const coverageRatio = 20.0 / 100.0;

    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setLogginToFile(FileOutput::csv);
    benchmark.setRoundsCount(repeatsNumber);
    benchmark.setDivisionFactor(TestTreeAdapter::lookupCount);
    benchmark.setTestingParam(50 * 1000 * 1000);

    benchmark.beginGroup();

    //16, 32, 40, 64, 96, 128, 256, 512, 1024
//    makeBenchmarkSet(benchmark, new TestTreeAdapter(16, coverageRatio), "b16_30M");
//
//	makeBenchmarkSet(benchmark, new TestTreeAdapter(32, coverageRatio), "b32_30M");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(40, coverageRatio), "b40_40M");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(64, coverageRatio), "b64_40M");  // WINS
//
//	makeBenchmarkSet(benchmark, new TestTreeAdapter(96, coverageRatio), "b96_30M");
//
//	makeBenchmarkSet(benchmark, new TestTreeAdapter(128, coverageRatio), "b128_30M");
//
//    makeBenchmarkSet(benchmark, new TestTreeAdapter(256, coverageRatio), "b256_30M");
//
//    makeBenchmarkSet(benchmark, new TestTreeAdapter(512, coverageRatio), "b512_30M");
//
//    makeBenchmarkSet(benchmark, new TestTreeAdapter(1024, coverageRatio), "b1024_30M");

    benchmark.endGroup();
}

void bTreeBinSearchTuning(Benchmaker &benchmark)
{
    int const repeatsNumber = 6;
    double const coverageRatio = 20.0 / 100.0;

    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setLogginToFile(FileOutput::none);
    benchmark.setRoundsCount(repeatsNumber);
    benchmark.setDivisionFactor(TestTreeAdapter::lookupCount);
    benchmark.setTestingParam(10 * 1000 * 1000);

    makeBenchmarkSet(benchmark, new TestTreeAdapter(1024, coverageRatio), "btree1024_bound100");

}

void splayTreeCompetition(Benchmaker &benchmark)
{
    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setRoundsCount(5);
    benchmark.setLogginToFile(FileOutput::none);
    double coverageRatio = 10.0 / 100.0;

    for (int i = 0; i < 5; i++)
    {
        benchmark.setRunnableObject(new TestTreeAdapter(tree::splay, coverageRatio));
        benchmark.setBenchmarkName("Splay tree. Coverage ratio = " + std::to_string(coverageRatio));
        benchmark.setTestingParam( 10 * 1000 * 1000);
        benchmark.makeBenchmark();
        coverageRatio += 5.0 / 100.0;
    }
}
