#pragma once

#include <vector>
#include <algorithm>

#include "iTree.h"

namespace Tree
{
//! @class VEBLayoutBinTree is a modification of AVL-like binary tree
//! implementation idea: each node has also children of host-key
//! that must decrease number of cachemisses
class AdvancedAvlTree : public ITree
{
public:
	AdvancedAvlTree();
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
		bool hasLeftChild;
		Type leftKey;
		bool hasRightChild;
		Type rightKey;
		Page* children[4];

		Page()
			: hasLeftChild(false)
			, hasRightChild(false)
		{
			for (int i = 0; i < 4; i++)
			{
				children[i] = nullptr;
			}
		}

		~Page()
		{
			for (int i = 0; i < 4; i++)
			{
				if (this->children[i] != nullptr)
				{
					delete this->children[i];
				}
			}
		}
	};

	Page *mRoot;
	std::vector<Type> mData;

	void makeSubTree(Page* &locRoot, int const leftBound, int const rightBound);
	bool findIn(Page *subTree, Type const &key) const;
};
}

