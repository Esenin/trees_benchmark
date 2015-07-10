#pragma once

#include "iTree.h"

namespace tree
{
template<size_t Page_Size = 40>
//! @class BTree is a pure implementation of B-Tree without any payload
class BTree : public ITree
{
public:
    explicit BTree() : mRoot(new Page())  {}

    virtual ~BTree()  { delete mRoot; }

    bool lookup(Type const &key) const;
    void insert(Type const &key);
    bool isEmpty() const;

protected:
    static int const binSearchBound = 100;
    //! pageSize shows the number of element per one page-node
    constexpr static size_t mPivot = Page_Size / 2;

    struct Page
    {
        int count = 0;
        Type* keys = new Type[Page_Size];
        bool isLeaf = true;
        Page** children = new Page* [Page_Size]{};

        ~Page()
        {
            for (auto i = 0; i < Page_Size; i++)
            {
                delete children[i];
            }
            delete[] keys;
            delete[] children;
        }
    };

    Page* mRoot;

    bool lookupRec(Page* localRoot, Type const &key) const;
    void insertFirst(Type const &key);
    void insertNonFull(Page* host, Type const &key);
    void splitChild(Page* host, int const &index);
};


template<size_t Page_Size>
bool BTree<Page_Size>::lookup(Type const &key) const
{
    return lookupRec(mRoot, key);
}

template<size_t Page_Size>
bool BTree<Page_Size>::lookupRec(BTree::Page* localRoot, Type const &key) const
{
    int left = 0;
    int right = localRoot->count - 1;
    while (right - left > binSearchBound)
    {
        int pivot = (left + right) / 2;
        if (localRoot->keys[pivot] > key)
        {
            right = pivot;
        }
        else if (localRoot->keys[pivot] < key)
        {
            left = pivot;
        }
        else
        {
            return true;
        }
    }

    for (int i = left; i <= right; i++)
    {
        if (localRoot->keys[i] == key)
            return true;

        if (localRoot->keys[i] > key)
        {
            if (localRoot->isLeaf)
                return false;

            return lookupRec(localRoot->children[i], key);
        }
    }
    return (localRoot->isLeaf) ? false : lookupRec(localRoot->children[localRoot->count], key);
}

template<size_t Page_Size>
void BTree<Page_Size>::insert(Type const &key)
{
    if (lookup(key))
        return;
    insertFirst(key);
}

template<size_t Page_Size>
bool BTree<Page_Size>::isEmpty() const
{
    return mRoot->count == 0;
}

template<size_t Page_Size>
void BTree<Page_Size>::insertFirst(Type const &key)
{
    Page* root = mRoot;
    if (root->count == Page_Size - 1)
    {
        Page* newNode = new Page();
        mRoot = newNode;
        newNode->isLeaf = false;
        newNode->count = 0;
        newNode->children[0] = root;
        splitChild(newNode, 0);
        insertNonFull(newNode, key);
    }
    else
    {
        insertNonFull(root, key);
    }
}

template<size_t Page_Size>
void BTree<Page_Size>::insertNonFull(BTree::Page* host, Type const &key)
{
    int i = host->count - 1;
    if (host->isLeaf)
    {
        while ((i >= 0) && (key < host->keys[i]))
        {
            host->keys[i + 1] = host->keys[i];
            i--;
        }
        host->keys[i + 1] = key;
        host->count++;
        return;
    }

    while ((i >= 0) && (key < host->keys[i]))
    {
        i--;
    }
    i++;
    if (host->children[i]->count == Page_Size - 1)
    {
        splitChild(host, i);
        if (key > host->keys[i])
        {
            i++;
        }
    }
    insertNonFull(host->children[i], key);
}

template<size_t Page_Size>
void BTree<Page_Size>::splitChild(BTree::Page* host, int const &index)
{
    Page* z = new Page();
    Page* y = host->children[index];
    z->isLeaf = y->isLeaf;

    z->count = mPivot - 1;
    for (int i = 0; i < mPivot - 1; i++)
    {
        z->keys[i] = y->keys[i + mPivot];
    }
    if (!y->isLeaf)
    {
        for (int i = 0; i < mPivot; i++)
        {
            z->children[i] = y->children[i + mPivot];
            y->children[i + mPivot] = nullptr;
        }
    }
    y->count = mPivot - 1;
    for (int i = host->count; i >= index + 1; i--)
    {
        host->children[i + 1] = host->children[i];
    }
    host->children[index + 1] = z;
    for (int i = host->count - 1; i >= index; i--)
    {
        host->keys[i + 1] = host->keys[i];
    }
    host->keys[index] = y->keys[mPivot - 1];
    host->count++;
}

}
