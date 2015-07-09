#pragma once

#include <vector>


#include "iTree.h"

namespace Tree
{
//! @class AdvancedAvlTree is a modification of AVL-like binary tree
//! implementation idea: each node has also children of host-key
//! that must decrease number of cachemisses
//!                         Page0
//!                 |-----------------------|
//!                 |         Key           |
//!                 |       /    \          |
//!                 |    lKey   rKey        |
//!                 |_______________________|
//!                   /     \      /     \
//!   sub-pages:    page1  page2  page3  page4
//!
//!
class AdvancedAvlTree : public ITree
{
public:
    AdvancedAvlTree() = default;
    //! @arg forward declaration of input data size. May speedup building because of it is static-tree
    explicit AdvancedAvlTree(size_t hintSize);
	~AdvancedAvlTree();

	void insert(Type const &key);
	bool lookup(Type const &key) const;
	bool isEmpty() const;
	//! Builds the tree from inputted  values due hastily implementation of "insert" method
	virtual void buildTree();
	virtual bool isBuildable() const;

protected:
	enum ChildIndex
	{
		leftLeftChild
		, leftRightChild
		, rightLeftChild
		, rightRightChild
	};

	struct Page
	{
		Type key;
		char hasLRChildren = 0;
		Type leftKey;
		Type rightKey;
		Page* children[4] {};

        ~Page()
        {
            for (Page* &child : children)
                delete child;
		}

        inline bool hasLeftChild() const  { return (hasLRChildren & 2) != 0; } // 00L0
		inline void setLeftKey(Type const &val)
        {
            leftKey = val;
            hasLRChildren |= 2;
        }
        inline bool hasRightChild() const { return (hasLRChildren & 1) != 0; } // 000R
		inline void setRightKey(Type const &val)
        {
            rightKey = val;
            hasLRChildren |= 1;
        }
	};

	Page* mRoot = nullptr ;
	std::vector<Type> mData;

	void makeSubTree(Page* &locRoot, int const leftBound, int const rightBound);
	bool findIn(Page* subTree, Type const &key) const;
};
}

