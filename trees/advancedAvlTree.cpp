#include "advancedAvlTree.h"

using namespace Tree;

Tree::AdvancedAvlTree::AdvancedAvlTree()
	: mRoot(nullptr)
{
}

AdvancedAvlTree::~AdvancedAvlTree()
{
	delete mRoot;
}

void AdvancedAvlTree::insert(Type const &key)
{
	mData.push_back(key);
}

bool AdvancedAvlTree::lookup(Type const &key) const
{
	return findIn(mRoot, key);
}

bool AdvancedAvlTree::isEmpty() const
{
	return mData.empty();
}

void AdvancedAvlTree::buildTree()
{
	if (isEmpty())
	{
		return;
	}
	std::sort(mData.begin(), mData.end());
	makeSubTree(mRoot, 0, mData.size() - 1);
}

bool AdvancedAvlTree::isBuildable() const
{
	return true;
}

void AdvancedAvlTree::makeSubTree(AdvancedAvlTree::Page* &locRoot, int const leftBound, int const rightBound)
{
	locRoot = new Page();
	if (mRoot == nullptr)
	{
		mRoot = locRoot;
	}
	int middle = (leftBound + rightBound) / 2;
	locRoot->key = mData.at(middle);

	// left half
	if (middle - leftBound > 0)
	{
		int leftPivot = (leftBound + middle) / 2;
		locRoot->leftKey = mData.at(leftPivot);
		locRoot->hasLeftChild = true;

		if (leftPivot - leftBound > 0)
		{
			makeSubTree(locRoot->children[leftLeftChild], leftBound, leftPivot - 1);
		}


		if (middle - leftPivot - 1 > 0)
		{
			makeSubTree(locRoot->children[leftRightChild], leftPivot + 1, middle - 1);
		}
	}

	// right
	if (rightBound - middle > 0)
	{
		int rightPivot = (rightBound + middle) / 2;
		if (rightPivot == middle)
		{
			rightPivot++;
		}
		locRoot->rightKey = mData.at(rightPivot);
		locRoot->hasRightChild = true;

		if (rightBound - rightPivot > 0)
		{
			makeSubTree(locRoot->children[rightRightChild], rightPivot + 1, rightBound);
		}


		if (rightPivot - middle - 1 > 0)
		{
			makeSubTree(locRoot->children[rightLeftChild], middle + 1, rightPivot - 1);
		}
	}
}

bool AdvancedAvlTree::findIn(AdvancedAvlTree::Page *subTree, Type const &key) const
{
	if (subTree == nullptr)
	{
		return false;
	}

	if (key < subTree->key)
	{
		if (!subTree->hasLeftChild)
		{
			return false;
		}
		if (key == subTree->leftKey)
		{
			return true;
		}
		ChildIndex index = (key < subTree->leftKey)? leftLeftChild : leftRightChild;
		return findIn(subTree->children[index], key);
	}

	if (key > subTree->key)
	{
		if (!subTree->hasRightChild)
		{
			return false;
		}
		if (key == subTree->rightKey)
		{
			return true;
		}
		ChildIndex index = (key < subTree->rightKey)? rightLeftChild : rightRightChild;
		return findIn(subTree->children[index], key);
	}

	return true;
}
