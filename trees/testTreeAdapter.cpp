#include "testTreeAdapter.h"
#include <chrono>
#include <iostream>
using namespace Tree;

TestTreeAdapter::TestTreeAdapter(TreeType const type, double coverageRatio)
    : mCoverageRatio(coverageRatio)
    , mBTreePageSize(0)
    , mTreeType(type)
{
}

TestTreeAdapter::TestTreeAdapter(size_t const &bTreeSize, double coverageRatio)
    : mCoverageRatio(coverageRatio)
    , mBTreePageSize(bTreeSize)
    , mTreeType(TreeType::b)
{
}

TestTreeAdapter::~TestTreeAdapter()
{
    freeTree();
}

void TestTreeAdapter::setParam(int const &param)
{
    if (param == 0)
        throw std::invalid_argument("Param must be positive numebr");
    mInputSize = param;
}

void TestTreeAdapter::prepare()
{
    auto saturateTree = [this] ()
    {
        Type leftPointer = 0;
        Type rightPointer = mInputSize - 1;
        while (rightPointer - leftPointer > 0)
        {
            mTree->insert(leftPointer++);
            mTree->insert(rightPointer--);
        }
        if (leftPointer == rightPointer)
            mTree->insert(leftPointer);

        if (mTree->isBuildable())
        {
            mTree->buildTree();
        }
    };

    createTree(mTreeType);

    std::thread parallelTreeFill(saturateTree);
    createRequests();
    parallelTreeFill.join();
}

void TestTreeAdapter::run()
{
    int const peekCount = 1000 * 1000;
    for (int i = 0, idx = 0; i < peekCount; i++)
    {
        if (!mTree->lookup(mRequests[idx++]))
        {
            throw std::logic_error("Cannot find value in tree. Suspicious");
        }
        idx = (idx >= mInputSize)? 0 : idx;
    }
}

void TestTreeAdapter::clear()
{
    freeTree();
    mRequests.clear();
}

void TestTreeAdapter::createTree(TreeType type)
{
    switch (type)
    {
    case b:
        mTree = new BTree(mBTreePageSize);
        break;
    case advancedAvlTree:
        mTree = new AdvancedAvlTree();
        break;
    case avlTree:
        mTree = new AVLTree();
        break;
    case stdRBTree:
        mTree = new StlMap();
        break;
    case vebLayoutTree:
        mTree = new VebLayoutTree();
        break;
    case splay:
        mTree = new SplayTreeWrap();
        break;
    default:
    {
        throw std::runtime_error("unexpected tree type");
    }
    }
}

void TestTreeAdapter::createRequests()
{
    mRequests.clear();
    mRequests.resize(mInputSize);
    mDistributer.configureStd(0, mInputSize - 1);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto statistics = mDistributer.fill(mRequests, mCoverageRatio);
    auto stopTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();

    std::cerr << "[i] Requests were created in " << duration << " [ms] Min-Max=" << statistics.minVal << "-"
              << statistics.maxVal << "\t Real / Declared coverage %: " << statistics.coverRatio << "/"
              << mCoverageRatio << ".\tSize = " << mRequests.size() << "\n";
}

void TestTreeAdapter::freeTree()
{
    delete mTree;
    mTree = nullptr;
}
