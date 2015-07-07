#include "bTree.h"

using namespace Tree;

BTree::BTree(size_t const &size)
	: mPageSize(size)
	, mPivot(size / 2)
	, mRoot(new Page(size))
{

}

BTree::~BTree()
{
	deleteSubDetour(mRoot);
	delete mRoot;
}

bool BTree::lookup(Type const &key) const
{
	return lookupRec(mRoot, key);
}

bool BTree::lookupRec(BTree::Page *localRoot, Type const &key) const
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

	for(int i = left; i <= right; i++)
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
	return (localRoot->isLeaf)? false : lookupRec(localRoot->children[localRoot->count], key);
}

void BTree::insert(Type const &key)
{
	if (lookup(key))
		return;
	insertFirst(key);
}

bool BTree::isEmpty() const
{
	return mRoot->count == 0;
}

void BTree::insertFirst(Type const &key)
{
	Page *root = mRoot;
	if (root->count == mPageSize - 1)
	{
		Page *newNode = new Page(mPageSize);
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

void BTree::insertNonFull(BTree::Page *host, Type const &key)
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
	if (host->children[i]->count == mPageSize - 1)
	{
		splitChild(host, i);
		if (key > host->keys[i])
		{
			i++;
		}
	}
	insertNonFull(host->children[i], key);
}

void BTree::splitChild(BTree::Page *host, int const &index)
{
	Page *z = new Page(mPageSize);
	Page *y = host->children[index];
	z->isLeaf = y->isLeaf;

	z->count = mPivot - 1;
	for (short i = 0; i < mPivot - 1; i++)
	{
		z->keys[i] = y->keys[i + mPivot];
	}
	if (!y->isLeaf)
	{
		for (short i = 0; i < mPivot; i++)
		{
			z->children[i] = y->children[i + mPivot];
		}
	}
	y->count = mPivot - 1;
	for (short i = host->count; i >= index + 1; i--)
	{
		host->children[i + 1] = host->children[i];
	}
	host->children[index + 1] = z;
	for (short i = host->count - 1; i >= index; i--)
	{
		host->keys[i + 1] = host->keys[i];
	}
	host->keys[index] = y->keys[mPivot - 1];
	host->count++;
}

void BTree::deleteSubDetour(BTree::Page *current)
{
	if (current->isLeaf)
		return;

	for (short i = 0; i <= current->count; i++)
	{
		if (current->children[i] == nullptr)
			return;
		if (current->children[i]->isLeaf)
		{
			delete current->children[i];
		}
		else
		{
			deleteSubDetour(current->children[i]);
			delete current->children[i];
		}
		current->children[i] = nullptr;
	}
}
