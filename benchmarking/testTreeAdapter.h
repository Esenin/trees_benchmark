#pragma once

#include <math.h>
#include <limits>
#include <vector>

#include "../AnyBenchmark/TestingFramework/include/iTestObject.h"
#include "normalDistGenerator.h"
#include "../trees/iTree.h"

namespace tree
{
enum TreeType
{
    b
    , advancedAvlTree
    , avlTree
    , stdRBTree
    , vebLayoutTree
    , splay
};

//! @class TestTreeAdapter is a wrap of Tree for a benchmark
class TestTreeAdapter : public benchmark::ITestObject
{
public:
    //! @arg type select what kind of tree will be used in testing
    //! @arg coverageRatio select requests' coverage percent relative to size param
    TestTreeAdapter(TreeType const type, double coverageRatio);
    TestTreeAdapter(size_t const &bTreeSize, double coverageRatio);
    virtual ~TestTreeAdapter();

    //! @arg param is size of test-input
    virtual void setParam(int const &param);
    virtual void prepare();
    virtual void run();
    virtual void clear();

protected:
    tree::ITree *mTree = nullptr;
    size_t mInputSize = 0;
    double mCoverageRatio;
    size_t mBTreePageSize;
    TreeType mTreeType;

    std::vector<Type> mRequests;
    NormalDistGenerator<Type> mDistributer;

    void createTree(TreeType type);
    void createRequests();
    void freeTree();
};
}
