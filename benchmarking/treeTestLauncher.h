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
	int const startSize = 1 * 1000 * 1000;
	int const maxSize = 30 * 1000 * 1000;
	int const stepSize = 10 * 1000 * 1000;

	benchmark.setRunnableObject(tree);
	benchmark.setBenchmarkName(name);
//    benchmark.setTestingParam(startSize, maxSize, stepSize);
	benchmark.setTestingParam(startSize);
	benchmark.makeBenchmark();
}

//! Complect of trees for performance test
void treesBenchmark(Benchmaker &benchmark)
{
    int const repeatsNumber = 3;
    double const coverageRatio = 20.0 / 100.0; //< 10% coverage for requests


    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setLogginToFile(FileOutput::none);
    benchmark.setRoundsCount(repeatsNumber);


    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::avlTree, coverageRatio), "Avl");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::vebLayoutTree, coverageRatio), "vebLayoutTree");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::splay, coverageRatio), "splay");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(64, coverageRatio), "btree64");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::advancedAvlTree, coverageRatio), "advancedAvlTree");

    makeBenchmarkSet(benchmark, new TestTreeAdapter(tree::stdRBTree, coverageRatio), "stlRBTree");
}

void bTreeParamTuning(Benchmaker &benchmark)
{
	benchmark.setRoundsCount(5);

    double const testCoverage = 25.0 / 100.0;

	makeBenchmarkSet(benchmark, new TestTreeAdapter(160, testCoverage), "btree160");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(128, testCoverage), "btree128");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(96, testCoverage), "btree96");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(64, testCoverage), "btree64");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(32, testCoverage), "btree32");
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
