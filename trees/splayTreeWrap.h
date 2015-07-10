#pragma once

#include "iTree.h"
#include "splayTree/splayTree.h"

namespace tree
{

class SplayTreeWrap : public ITree
{
public:
    SplayTreeWrap() : mSplayTree(std::numeric_limits<Type>::max())
    {}

    void insert(Type const &key)
    {
        mSplayTree.insert(key);
    }

    bool lookup(Type const &key) const
    {
        return mSplayTree.contains(key);
    }

    bool isEmpty() const
    {
        return mSplayTree.isEmpty();
    }

protected:
    splay2::SplayTree<Type> mSplayTree;
};


}