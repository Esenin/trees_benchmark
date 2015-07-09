//! @brief  Unit testing for trees
//!

#include <iostream>
#include <limits>
#include <vector>
#include <set>
#include <chrono>
#include <assert.h>
#include <thread>

#include "stlMap.h"
#include "advancedAvlTree.h"
#include "avlTree.h"
#include "vebLayout.h"
#include "splayTreeWrap.h"
#include "bTree.h"

using namespace std;
using namespace Tree;

namespace
{
class Timer
{
public:
    typedef decltype(std::chrono::high_resolution_clock::now()) TimeValue;
    static TimeValue  mStartTime;

    static void start()
    { Timer::mStartTime = std::chrono::high_resolution_clock::now(); }

    static long long stop()
    {
        auto stopTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - Timer::mStartTime).count();
    }
    static long long stopUS()
    {
        auto stopTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(stopTime - Timer::mStartTime).count();
    }
};

typename Timer::TimeValue Timer::mStartTime;

} // anonymous namespace


typedef std::vector<ITree *> Trees;


void fastCheckTree(ITree *tree)
{
    std::set<Type> requests = {0, 1, 5, 6, 9, 13};
    for (Type const &r : requests)
        tree->insert(r);

    if (tree->isBuildable())
        tree->buildTree();

    for (Type i = 0; i < 20; i++)
    {
        assert(tree->lookup(i) == (requests.count(i) > 0));
    }
    delete tree;
}

void fastCheckTree()
{
    cerr << "Fast check started\n";
    Timer::start();
    fastCheckTree(new StlMap());
    fastCheckTree(new AVLTree());
    fastCheckTree(new AdvancedAvlTree());
    fastCheckTree(new VebLayoutTree());
    fastCheckTree(new SplayTreeWrap());
    fastCheckTree(new BTree());

    cerr << "Fast check finished in "<< Timer::stop() <<" [ms]\n";
}

vector<ITree*> createAllTrees(size_t const hintSize = 0)
{
    Trees trees;
    trees.push_back(new Tree::StlMap());
    trees.push_back(new Tree::AdvancedAvlTree(hintSize));
    trees.push_back(new Tree::AVLTree());
    trees.push_back(new Tree::VebLayoutTree(hintSize));
    trees.push_back(new Tree::SplayTreeWrap());
    trees.push_back(new Tree::BTree());
    return trees;
}

void parallelBuildTrees(Trees const &trees)
{
    vector<thread> builders;
    for (auto const t : trees)
        if (t->isBuildable())
            builders.push_back(thread(&ITree::buildTree, t));

    for (auto &th : builders)
        th.join();
}

void highlySparsedTest()
{
    std::cerr << "Highly sparsed test started\n";
    Timer::start();

    int const testSize = 500;
    int const checkSize = testSize * 10000;
    auto const trees = createAllTrees(testSize);

    set<Type> requests;
    for (int i = 0; i < testSize; i++)
    {
        Type const value = rand() % numeric_limits<Type>::max() - 1;
        requests.insert(value);
        for (auto const t : trees)
            t->insert(value);
    }

    parallelBuildTrees(trees);

    for (auto i = 0; i < checkSize; i++)
        for (auto const t : trees)
        {
            Type const val = rand() % numeric_limits<Type>::max() - 1;
            if (t->lookup(val) != (requests.find(val) != requests.end()))
                throw logic_error("Sparse test failed");
        }

    for (auto const t : trees)
        delete t;
    cerr << "Highly sparsed test finished successfully in "<< double(Timer::stop()) / 1000 <<" [s]\n";
}



//! some simple tests for validation of trees
void OneMTreePretests()
{
    cerr << "1M-sized test started\n";
    Timer::start();

    int const testSize = 1000000;
    auto const trees = createAllTrees(testSize);


    std::set<int> requests;
    for (int i = 0; i < testSize / 3; ++i)
    {
        int value = rand() % testSize;
        requests.insert(value);
        for (Tree::ITree *tree : trees)
            tree->insert(value);
    }

    parallelBuildTrees(trees);

    for (int i = testSize; i >= 0; --i)
    {
        bool answer = requests.find(i) != requests.end();
        for (Tree::ITree *tree : trees)
        {
            if(tree->lookup(i) != answer)
            {
                assert(false);
                throw std::runtime_error("1M-sized test failed");
            }
        }
    }

    for (Tree::ITree *tree : trees)
        delete tree;
    cerr << "1M-sized test finished successfully in "<< double(Timer::stop()) / 1000 <<" [s]\n";
}


int main()
{
    Timer::start();
    this_thread::sleep_for(chrono::milliseconds(100));
    srand(Timer::stopUS());
    try
    {
        fastCheckTree();
        OneMTreePretests();
        highlySparsedTest();
    } catch (std::exception &e)
    {
        cout << "Test didn't pass. e: " << e.what();
    }

    cout << "All tests passed!\n";
}