#pragma once

#include "iTree.h"

namespace Tree
{
//! @class BTree is a pure implementation of B-Tree without any payload
class BTree : public ITree
{
public:
	explicit BTree(size_t const &size = 40);
	virtual ~BTree();

	bool lookup(Type const &key) const;
	void insert(Type const &key);
	bool isEmpty() const;

protected:
	static int const binSearchBound = 2000;
	//! pageSize shows the number of element per one page-node
	size_t const mPageSize;
	int const mPivot;
	struct Page
	{
		int count;
		Type *keys;
		bool isLeaf;
		Page* *children;

		Page(int const &size)
			: count(0)
			, isLeaf(true)

		{
			children = new Page*[size];
			keys = new Type[size];
			for (int i = 0; i < size; i++)
			{
				children[i] = nullptr;
			}
		}
		~Page()
		{
			delete[] keys;
			delete[] children;
		}

	};

	Page *mRoot;

	bool lookupRec(Page *localRoot, Type const &key) const;
	void insertFirst(Type const &key);
	void insertNonFull(Page *host, Type const &key);
	void splitChild(Page *host, int const &index);
	void deleteSubDetour(Page *current);
};

}
