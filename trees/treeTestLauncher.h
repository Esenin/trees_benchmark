#pragma once

#include <set>
#include <assert.h>
#include <string>
#include <iostream>

#include "../AnyBenchmark/TestingFramework/include/benchmaker.h"
#include "testTreeAdapter.h"

using Tree::TestTreeAdapter;
using namespace benchmark;

//! some simple tests for validation of trees
void treePretests()
{
    std::cerr << "Pretest for trees started\n";
	int const testSize = 1000000;
	std::vector<Tree::ITree*> trees;
	trees.push_back(new Tree::StlMap());
	trees.push_back(new Tree::AdvancedAvlTree());
	trees.push_back(new Tree::AVLTree());
	trees.push_back(new Tree::VebLayoutTree());
	trees.push_back(new Tree::SplayTreeWrap());
	trees.push_back(new Tree::BTree());

	std::set<int> requests;
	for (int i = 0; i < testSize / 3; ++i)
	{
		int value = rand() % testSize;
		requests.insert(value);
		for (Tree::ITree *tree : trees)
		{
			tree->insert(value);
		}
	}

	for (Tree::ITree *tree : trees)
	{
		if (tree->isBuildable())
			tree->buildTree();
	}

	for (int i = testSize; i >= 0; --i)
	{
		bool answer = requests.find(i) != requests.end();
		for (Tree::ITree *tree : trees)
		{
			if(tree->lookup(i) != answer)
            {
                assert(false);
                throw std::runtime_error("pretest failed");
            }
		}
	}

	for (Tree::ITree *tree : trees)
	{
		delete tree;
	}
    std::cerr << "Pretest finished successfully\n";
}

//! that is a benchmark config
void makeBenchmarkSet(Benchmaker &benchmark, TestTreeAdapter *tree, std::string const &name = "")
{
	int const startSize = 1 * 1000 * 1000;
	int const maxSize = 30 * 1000 * 1000;
	int const stepSize = 10 * 1000 * 1000;

	benchmark.setRunnableObject(tree);
	benchmark.setBenchmarkName(name);
//    benchmark.setTestingParam(startSize, maxSize, stepSize);
	benchmark.setTestingParam(maxSize / 2);
	benchmark.makeBenchmark();
}

//! Complect of trees for performance test
void treesBenchmark(Benchmaker &benchmark)
{
    treePretests();

    int const repeatsNumber = 3;
    double const coverageRatio = 10.0 / 100.0; //< 10% coverage for requests


    benchmark.setMeasureType(MeasureType::cpuTime);
    benchmark.setLogginToFile(FileOutput::none);
    benchmark.setRoundsCount(repeatsNumber);

	makeBenchmarkSet(benchmark, new TestTreeAdapter(Tree::avlTree, coverageRatio), "Avl");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(Tree::vebLayoutTree, coverageRatio), "vebLayoutTree");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(Tree::splay, coverageRatio), "splay");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(64, coverageRatio), "btree64");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(Tree::advancedAvlTree, coverageRatio), "advancedAvlTree");

	//makeBenchmarkSet(benchmark, new TestTreeAdapter(Tree::stdRBTree), "stlRBTree");
}

void bTreeParamTuning(Benchmaker &benchmark)
{
	treePretests();
	benchmark.setRoundsCount(5);

    double const testCoverage = 25.0 / 100.0;

	makeBenchmarkSet(benchmark, new TestTreeAdapter(160, testCoverage), "btree160");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(128, testCoverage), "btree128");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(96, testCoverage), "btree96");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(64, testCoverage), "btree64");

	makeBenchmarkSet(benchmark, new TestTreeAdapter(32, testCoverage), "btree32");
}
